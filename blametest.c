#include "blame.h"
#include <stdio.h>


int main()
{
	binit();
	char* a = bmalloc(20);
	printf("%d\n", (int)a);
	a = brealloc(a, 40);
	printf("%d\n", (int)a);
	bfree(a);
	blame();
	return 0;
}
