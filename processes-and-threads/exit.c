#include <stdio.h>
#include <stdlib.h>

void myexit1()
{
	printf("first exit handler\n");
}

void myexit2()
{
	printf("second exit handler\n");
}

int main()
{
	if (atexit(myexit2) != 0) { // 프로세스가 종료될때 실행
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) { // 프로세스가 종료될때 실행
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) { // 프로세스가 종료될때 실행
		perror("atexit");
		exit(1);
	}

	printf("main is done\n"); 

	return 0;
}
