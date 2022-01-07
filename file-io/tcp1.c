#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *src, *dst;
	int ch;

	if (argc != 3) { // 필요한 인자가 잘못되면 에러 출력
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL) { // 첫번째 파일 열기, 텍스트 읽기모
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wt")) == NULL) { // 두번째 파일 열기, 텍스트 쓰기모드
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF) { // 문자 단위로 파일 읽기, 파일의 끝이면 반복 중단
		fputc(ch, dst); // 문자 단위로 쓰기
	}

	fclose(src); // 파일 닫기
	fclose(dst); // 파일 닫기

	return 0;
}


