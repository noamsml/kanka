#include "blame.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef BLAME
#define TABLESIZE 1024

struct blame_tablenode
{
	void* ptr;
	int line;
	const char* file;
	int line_re;
	const char* file_re;
	int line_freed; //for bless table
	const char* file_freed;
	int size;
	struct blame_tablenode* next;
};

typedef struct blame_tablenode* node;

node* blametable;

node new_node(void* ptr, const char* file, int line, int size, node next)
{
	node rval = malloc(sizeof(struct blame_tablenode));
	rval->ptr = ptr;
	rval->file = file;
	rval->line = line;
	rval->size = size;
	rval->line_re = 0;
	rval->line_freed = 0;
	rval->next= next;
	return rval;
}

void binit()
{
	int i;
	//never really deallocated, but owell
	blametable = malloc(TABLESIZE * sizeof(node));
	for (i = 0; i < TABLESIZE; i++)
	{
		blametable[i] = NULL;
	}
}

void* __bmalloc(int size, const char* filename, int lineno)
{
	void* ptr = malloc(size);
	int index = (int)ptr % TABLESIZE;
	blametable[index] = new_node(ptr, filename, lineno, size, blametable[index]);
	return ptr;
}

void __bfree(void* ptr, const char* filename, int lineno)
{
	int index = (int)ptr % TABLESIZE;
	node prev = NULL;
	node cur = blametable[index];
	while (cur)
	{
		if (cur->ptr == ptr)
		{
			if (prev) prev->next = cur->next;
			else blametable[index] = cur->next;
			free(cur->ptr);
			free(cur);
			return;
		}
		prev = cur;
		cur = cur->next;
	}
	
	printf("WARNING: bfree: %s/%d : pointer %d not found.\n", filename, lineno, ptr);
}

void* __brealloc(void* ptr, int size, const char* filename, int lineno)
{
	int index = (int)ptr % TABLESIZE;
	node cur = blametable[index];
	while (cur)
	{
		if (cur->ptr == ptr)
		{
			cur->ptr = realloc(cur->ptr, size);
			cur->size = size;
			cur->line_re = lineno;
			cur->file_re = filename;
			return cur->ptr;
		}
		cur = cur->next;
	}
	
	printf("WARNING: brealloc: %s/%d : pointer %d not found.\n", filename, lineno, ptr);
}

int blame()
{
	//a bit *too* slow, I suspect
	int i;
	node cur;
	int blamable = 0;
	
	for (i = 0; i < TABLESIZE; i++)
	{
		cur = blametable[i];
		if (cur && !blamable) 
		{
			blamable = 1; 
			printf("\nUNFREED MEMORY FOUND!\n%-5s %-10s %-5s\n", "SIZE", "FILENAME", "LINE");
		}
		while (cur)
		{
			printf("%-5d %-10s %-5d\n", cur->size, cur->file, cur->line);
			cur = cur->next;
		}
	}
	return blamable;
}

#else

void* __bmalloc(int size, const char* file, int line) { return malloc(size); }
void __bfree(void* ptr, const char* file, int line) { free(ptr); }
void* __brealloc(void* ptr, int size, const char* file, int line) { return realloc(ptr, size); }

void binit() {}
int blame() {return 0;}

#endif

