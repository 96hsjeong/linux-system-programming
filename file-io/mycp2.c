#include <stdio.h>

#define MAX_BUF 1024

int main(int argc, char *argv[])
{
	FILE *src, *dst;
	char buf[MAX_BUF];
	int count;

	if (argc != 3) { // 필요한 인자가 잘못되면 에러 출력
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rb")) == NULL) { // 첫번째 파일 열기, 바이너리파일 읽기
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wb")) == NULL) { // 두번째 파일 열기 , 바이너리파일 쓰기
		perror("fopen");
		exit(1);
	}

	while ((count = fread(buf, 1, MAX_BUF, src)) > 0) { // 버퍼 크기만큼 파일 읽기
		fwrite(buf, 1, count, dst); // 목적지 파일에 버퍼 쓰기
	}

	fclose(src); // 파일 닫기
	fclose(dst); // 파일 닫기 

	return 0;
}
