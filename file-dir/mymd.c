#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (mkdir(argv[1], 0755) < 0) { // 디렉토리 생성
		perror("mkdir");
		exit(1);
	}

	return 0;
}
