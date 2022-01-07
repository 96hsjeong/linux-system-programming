#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if (symlink(argv[1], argv[2]) < 0) { // 심볼릭링크 파일 생성
		perror("link");
		exit(1);
	}

	return 0;
}
