#include "mstring.h"
#include <stdio.h>

int main()
{
	string s;
	s = mstr_new(50);
	mstr_ccopy(s,"Hello world");
	printf("%s", s->str);
	mstr_achr(s, '\n');
	printf("%s", s->str);
	string s2 = mstr_cclone("Suckers\n");
	string s3 = mstr_clone(s2);
	mstr_amstr(s, s3);
	mstr_astr(s, "Bahaha\n");
	printf("%s", s->str);
	mstr_destroy(s);
	mstr_destroy(s2);
	mstr_destroy(s3);
}
