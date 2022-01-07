#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (rmdir(argv[1]) < 0) { // 디렉토리 지우기
		perror("rmdir");
		exit(1);
	}

	return 0;
}
