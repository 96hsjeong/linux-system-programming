#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

	if (rename(argv[1], argv[2]) < 0) { // 파일 이름 바꾸기
		perror("rename");
		exit(1);
	}

	return 0;
}
