#include <stdio.h>

extern "C"
void test(int i)
{
	printf("i: %d", i);
}
