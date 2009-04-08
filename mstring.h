#ifndef __MSTRING
#define __MSTRING

struct _string
{
	char* str;
	int len;
	int maxlen;
};

typedef struct _string* string;

string mstr_new(int);
void mstr_destroy(string);
string mstr_clone(string);
string mstr_cclone(const char*);

//Appends are O(1)
int mstr_achr(string, char); //char to add => success(1)/fail(0)
int mstr_astr(string, const char*); //str to add => success(1)/fail(0)
int mstr_amstr(string, string); 

//Prepends are O(N)
//int mstr_pchr(string, char);
//int mstr_pstr(string, char*);
//int mstr_pmstr(string, string);

void mstr_lower(string);
void mstr_upper(string);

int mstr_compare(string, string);
int mstr_ccompare(string, const char*);

void mstr_zero(string);

int mstr_copy(string, string);
int mstr_ccopy(string, const char*);

int mstr_resize(string, int); //Resize the mstr
int mstr_grow(string, int); //Grow the mstr

void mstr_snip(string, int, int);
string mstr_substr(string, int, int);
string mstr_nsubstr(string, int, int, int);
#endif
