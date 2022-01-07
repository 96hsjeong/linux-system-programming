#include <stdio.h>
#include <stdlib.h>

int main()
{
	int status;

	if ((status = system("date")) < 0) { // system 명령 수행
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if ((status = system("nosuchcommand")) < 0) { // system 명령 수행 
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if ((status = system("who; exit 44")) < 0) {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status); // system 명령 수행 

	return 0;
}
