#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 128

int main()
{
	int n, fd1[2], fd2[2];
	pid_t pid;
	char buf[MAX_BUF];

	if (pipe(fd1) < 0 || pipe(fd2) < 0) { // 파이프 생성 
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid == 0) { // Child Process
		close(fd1[1]); // child fd1 write close
		close(fd2[0]); // child fd2 read close

		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd1[0], buf, MAX_BUF)) < 0) { // 파이프에서 읽기 (파이프에서 읽을게 없으면 대기 상태로 전환)
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n); // buf 출력 

		// 숙제2. child -> parent 
		strcpy(buf, "Goodbye, World!\n");
		printf("Child: Send data to parent\n");
		write(fd2[1], buf, strlen(buf)+1); // fd2 파이프에 buf 쓰기 

	}
	else { // Parent Process 
		close(fd1[0]); // parent fd1 read close
		close(fd2[1]); // parent fd2 write close

		strcpy(buf, "Hello, World!\n"); // buf에 문자열 복사 
		printf("Parent: Send data to child\n");
		write(fd1[1], buf, strlen(buf)+1); // 파이프에 buf 쓰기

		// 숙제2. child -> parent
		printf("Parent : Wait for child to send data\n");
		if ((n = read(fd2[0], buf, MAX_BUF)) < 0) { // fd2 파이프에서 읽기 
			perror("read");
			exit(1);
		}
		printf("Parent : Received data from child: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
	}

	exit(0);

	return 0;
}

