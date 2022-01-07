#include <stdio.h>

int main(void)
{
	char c, s[80]; // c: 1byte char형, s[80]: char형 배열 (배열의 크기: 80)
	int i; long l; // i: 4byte 정수형, l: 4byte 정수형
	float f; double d; // f: 4byte 실수형, d: 8byte 실수형

	// %c: char
	// %s: 문자열
	// %d: int
	// %ld: long
	// %f: float
	// %lf: double
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); // 입력 함수
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d); // 출력 함수

	c = getchar(); // 문자 입력 함수
	putchar(c); // 문자 출력 함수

	gets(s); // 문자열 입력 함수
	puts(s); // 문자열 출력 함수

	return 0;
}
