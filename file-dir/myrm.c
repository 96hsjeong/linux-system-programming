#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;

	if (argc == 1) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; i++) { // 두번째 인자(파일)부터 파일 지우기
		if (remove(argv[i]) < 0) {
			perror("remove");
			exit(1);
		}
	}

	return 0;
}
