#include <stdio.h>

int main(void)
{
	char c = 'a', s[] = "hello"; // c: 1byte char형, s[]: nbyte char형 배열 (6byte)
	int i = 100; long l = 99999; // int: 4byte 정수, long: 4byte 정수형 (32bit 운영체제인 경우)
	float f = 3.14; double d = 99.999; // float: 4byte 실수형, double: 8byte 실수형
	int *p = &i; // *p: 포인터 변수, &i: 변수 i의 주소값

	// %c: char
	// %s: char*
	// %d: int
	// %#X: 16진수 정수 대문자로 표현 (#: 0X 붙임)
	// %ld: long
	// %.4f: float (소수점 넷재짜리까지 표현)
	// %.2lf: double (소수점 둘째짜리까지 표현)
	// %p: 포인터 주소값
	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
	putchar(c); // 문자 출력 함수
	puts(s); // 문자열 출력 함수

	return 0;
}
