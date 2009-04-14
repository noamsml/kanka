#include "mstring.h"
#include "blame.h"
#include <string.h>
#include <stdlib.h>

//Constructors and destructors
//=============================
string mstr_new(int size)
{
	string s = bmalloc(sizeof(struct _string));
	s->maxlen = size;
	s->len = 0;
	s->str = bmalloc(s->maxlen+1);
	*s->str = 0;
	return s;
}

void mstr_destroy(string s)
{
	bfree(s->str);
	bfree(s);
}

string mstr_clone(string ms)
{
	string s = bmalloc(sizeof(struct _string));
	s->maxlen = ms->maxlen;
	s->len = ms->len;
	s->str = bmalloc(s->maxlen+1);
	strcpy(s->str, ms->str);
	return s;
}

//Avoid using if possible
string mstr_cclone(const char* str)
{
	int l = strlen(str);
	string s = bmalloc(sizeof(struct _string));
	s->maxlen = l;
	s->len = l;
	s->str = bmalloc(s->maxlen+1);
	strcpy(s->str, str);
	return s;
}


//Methods
//========

//Append
//-------
int mstr_achr(string s, char c)
{
	if (s->len == s->maxlen) return 0;
	s->str[s->len] = c;
	s->len++;
	return 1;
}

int mstr_astr(string s, const char* cs)
{
	int l = strlen(cs);
	if (s->len + l > s->maxlen) return 0;
	while (*cs)
	{
		s->str[s->len] = *cs;
		cs++;
		s->len++;
	}
	s->str[s->len] = 0;
	return 1;
}

int mstr_amstr(string s, string s2)
{
	char* cs = s2->str;
	if (s->len + s2->len > s->maxlen) return 0;
	while (*cs)
	{
		s->str[s->len] = *cs;
		cs++;
		s->len++;
	}
	s->str[s->len] = 0;
	return 1;
}

//Case Changes
//------------

void mstr_lower(string s)
{
	char* sp = s->str;
	while (*sp)
	{
		if (*sp > 'A' && *sp < 'Z')
		{
			*sp = *sp - 'A' + 'a';
		}
	}
}

void mstr_upper(string s)
{
	char* sp = s->str;
	while (*sp)
	{
		if (*sp > 'a' && *sp < 'z')
		{
			*sp = *sp - 'a' + 'A';
		}
	}
}

//Compare
//--------

int mstr_compare(string s, string s2)
{
	return !strcmp(s->str, s2->str);
}

int mstr_ccompare(string s, const char* s2)
{
	return !strcmp(s->str, s2);
}

//Reset
//-----
void mstr_zero(string s)
{
	s->len = 0;
	*s->str = 0;
}

//Copy
//----

int mstr_copy(string s, string s2)
{
	if (s2->len > s->maxlen) return 0;
	strcpy(s->str, s2->str);
	s->len = s2->len;
}

int mstr_ccopy(string s, const char* s2)
{
	int l = strlen(s2);
	if (l > s->maxlen) return 0;
	strcpy(s->str, s2);
	s->len = l;
}

//Resize
//------

int mstr_resize(string s, int len)
{
	if (s->len > len) return 0;
	s->maxlen = len;
	s->str = realloc(s->str, s->maxlen);
	return 1; 
}

int mstr_grow(string s, int off)
{
	return mstr_resize(s, s->maxlen+off);
}

//Substring
//---------

//There's some code duplication in these functions, but I can't think
//Of a way around it that wouldn't impair functionality or slightly hurt
//Performance

void mstr_snip(string s, int start, int end)
{
	//Fixin'
	if (start < 0) start = 0;
	if (start > s->len) start = s->len;
	if (end < 0 || start > s->len) end = s->len;
	s->len = end-start;
	memmove(s->str, s->str+start, s->len);
	s->str[s->len] = 0;
}

//This function is slightly unsafe. Use it only if you know what you're doing.
string mstr_nsubstr(string s, int start, int end, int size)
{
	if (start < 0) start = 0;
	if (start > s->len) start = s->len;
	if (end < 0 || start > s->len) end = s->len;
	if (size < end-start) return NULL;
	string ret = mstr_new(size);
	memcpy(ret->str, s->str + start, end-start);
	ret->len = end-start;
	ret->str[ret->len] = 0;
	return ret;
}

string mstr_substr(string s, int start, int end)
{
	if (start < 0) start = 0;
	if (start > s->len) start = s->len;
	if (end < 0 || start > s->len) end = s->len;
	return mstr_nsubstr(s, start, end, end-start);
}
