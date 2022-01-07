#include <stdio.h>

// str 문자열의 첫번째 문자부터 문자마다 len++, 문자가 \0이면 반복 중단하고 len 리턴
int strlen_p(char *str)
{
	int len = 0;
	while (*str++) {
		len++;
	}
	return len;
}

// str 문자열의 첫번재 인덱스부터 인덱스 i를 증가시키며 \0이 나오면 중단하고 i 리턴
int strlen_a(char str[])
{
	int i;
	for (i = 0; str[i] != '\0'; i++);
	return i;
}

// *src의 값이 \0이 나올때까지 반복하며 dst에 src 값을 저장하고, 마지막으로 *dst에 \0 저장
void strcpy_p(char *dst, char *src)
{
	while (*src) {
		*dst++ = *src++;
	}
	*dst = *src;
}

// src의 인덱스 i를 증가하며 dst[i]에 src[i] 저장, \0이 나오면 반복중단하고 dst[i]에 \0 저장
void strcpy_a(char dst[], char src[])
{
	int i;
	for (i = 0; src[i] != '\0'; i++)
		dst[i] = src[i];
	dst[i] = src[i];
}

// 반복문으로 *dst를 \0까지 이동후 한자리 앞으로 이동, 이어서 src값을 dst에 저장하고, dst 끝에 \0 저장
void strcat_p(char *dst, char *src)
{
	while (*dst++);
	dst--;

	while (*src)
		*dst++ = *src++;
	*dst = *src;
}

// dst 인덱스 i를 dst 문자열 마지막 인덱스까지 증가, dst 인덱스에 src 인덱스를 더한 값을 인덱스로 dst에 src 값 저장
void strcat_a(char dst[], char src[])
{
	int i, j;
	
	for (i = 0; dst[i] != '\0'; i++);
	
	for (j = 0; src[j] != '\0'; j++)
		dst[i + j] = src[j];
	dst[i + j] = src[j];
}

// dst와 src의 각 자리를 아스키값으로 비교해서 c1 < c2이면 -1, c1 > c2이면 1을 리턴하고, c1과 c2가 \0이면 반복중단하고 0 리턴
int strcmp_p(char *dst, char *src)
{
	char c1, c2;
	while (1)
	{
		c1 = *dst++;
		c2 = *src++;
		if (c1 < c2)
			return -1;
		else if (c1 > c2)
			return 1;
		
		if (c1 == '\0' && c2 == '\0')
			break;
	}

	return 0;
}

// dst 인덱스 값을 증가시키며, dst와 src의 각 자리를 아스키값으로 비교, dst[i] < src[i]이면 -1, dst[i] > src[i]이면 1을 리턴
// dst[i]가 \0이면 반복 중단하고 src[i]가 \0이 아니면 src에 문자가 남아있으므로 -1 리턴, src[i]가 \0이면 0 리턴
int strcmp_a(char dst[], char src[])
{
	int i;
	
	for (i = 0; dst[i] != '\0'; i++)
	{
		if (dst[i] < src[i])
			return -1;
		else if (dst[i] > src[i])
			return 1;
	}

	if (src[i] != '\0')
		return -1;

	return 0;
}

int main(void)
{
	int len1, len2;
	char str1[20], str2[20];
	int strcmp1, strcmp2;

	// 문자열 길이
	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	// 문자열 복사
	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	// 문자열 연결
	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);

	// 문자열 비교
	strcmp1 = strcmp_p("hello", "hello");
	strcmp2 = strcmp_a("hello", "hello");
	printf("strcmp: p=%d, a=%d\n", strcmp1, strcmp2);

	strcmp1 = strcmp_p("hello", "world");
	strcmp2 = strcmp_a("hello", "world");
	printf("strcmp: p=%d, a=%d\n", strcmp1, strcmp2);

	strcmp1 = strcmp_p("world", "hello");
	strcmp2 = strcmp_a("world", "hello");
	printf("strcmp: p=%d, a=%d\n", strcmp1, strcmp2);

	strcmp1 = strcmp_p("hello", "helloworld");
	strcmp2 = strcmp_a("hello", "helloworld");
	printf("strcmp: p=%d, a=%d\n", strcmp1, strcmp2);

	return 0;
}
