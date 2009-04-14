#ifndef __BLAMEALLOC
#define __BLAMEALLOC

#define bmalloc(s) (__bmalloc((s), __FILE__, __LINE__))
#define bfree(p) (__bfree((p), __FILE__, __LINE__)) //May be used for bless table
#define brealloc(p,s) (__brealloc((p), (s),  __FILE__, __LINE__))

void* __bmalloc(int, const char*, int);
void __bfree(void*, const char*, int);
void* __brealloc(void*, int, const char*, int);

void binit();
int blame();
#endif


