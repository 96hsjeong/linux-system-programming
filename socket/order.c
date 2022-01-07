#include <stdio.h>

int main()
{
	int a = 0x12345678;
	char *p = (char *)&a; // little endian : 끝부분이 작은 주소값에 들어감.
#if 1
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
	printf("Address %p: %#x\n", p, *p); p++;
#else
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
	printf("Address %p: %#x\n", p, *p++);
#endif
}
