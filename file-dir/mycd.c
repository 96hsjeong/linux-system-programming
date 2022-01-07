#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (chdir(argv[1]) < 0) { // 디렉토리 변경
		perror("chdir");
		exit(1);
	}

	return 0;
}
