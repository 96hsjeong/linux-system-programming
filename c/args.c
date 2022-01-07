#include <stdio.h>

// argc: main함수의 인자 수
// *argv[]: char형 더블 포인터 변수
int main(int argc, char *argv[])
{
	int i; // 4byte 정수형
	for (i = 0; i < argc; i++) // 반복문 (argc의 값만큼 반복)
		printf("argv[%d]=%s\n", i, argv[i]); // argv의 i번째 값 출력

	return 0;
}
