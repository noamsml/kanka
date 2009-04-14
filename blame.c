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
	
	printf("WARNING: bfree: %s/%d : pointer %d not found.\n", filename, lineno, (int)ptr);
}

void* __brealloc(void* ptr, int size, const char* filename, int lineno)
{
	int index = (int)ptr % TABLESIZE;
	node cur = blametable[index];
	node prev = NULL;
	while (cur)
	{
		if (cur->ptr == ptr)
		{
			//1. remove from the table so we can reinsert correctly
			if (prev) prev->next = cur->next;
			else blametable[index] = cur->next;
			
			//2. reallocate and normalize our data
			cur->ptr = realloc(cur->ptr, size);
			cur->size = size;
			cur->line_re = lineno;
			cur->file_re = filename;
			index = (int)cur->ptr % TABLESIZE;
			
			//3. reinsert
			cur->next = blametable[index];
			blametable[index] = cur;
			
			return cur->ptr;
		}
		prev = cur;
		cur = cur->next;
	}
	
	printf("WARNING: brealloc: %s/%d : pointer %d not found.\n", filename, lineno, (int)ptr);
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
			printf("\nUNFREED MEMORY FOUND!\n%-5s %-12s %-20s %-5s %-20s %-5s\n", "SIZE", "ADDRESS", "FILENAME", "LINE", "RFILE", "RLINE");
		}
		while (cur)
		{
			if (cur->line_re) printf("%-5d %-12d %-20s %-5d %-20s %-5d \n", cur->size, (int)cur->ptr, cur->file, cur->line, cur->file_re, cur->line_re);
			else printf("%-5d %-12d %-20s %-5d\n", cur->size, (int)cur->ptr, cur->file, cur->line);
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

