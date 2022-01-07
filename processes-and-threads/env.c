#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) 
{
	int i;
	char **p;
	extern char **environ; // eviron 변수 참조

	printf("List command-line arguments\n");
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");
	for (p = environ; *p != NULL; p++) { // 환경변수 출력
		printf("%s\n", *p);
	}

	printf("\n");
	printf("List environment variables from envp variable\n");
	for (p = envp; *p != NULL; p++) { // 환경변수 출력
		printf("%s\n", *p);
	}

	return 0;
}
