#include <stdio.h>

#define MAX_BUF 256 // 버퍼 최대 크기

int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[MAX_BUF];
	int line;

	if (argc != 2) { // 필요한 인자가 잘못되면 에러 출력
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "rt")) == NULL) { // 첫번째 파일 열기 
		perror("fopen");
		exit(1);
	}

	line = 1; // 첫번째 줄

	while (fgets(buf, MAX_BUF, fp)) { // 줄단위로 파일 읽기
		printf("%4d: %s", line++, buf); // 버퍼 앞에 줄번호 붙여서 출력
	}

	fclose(fp); // 파일 닫기

	return 0;
}

