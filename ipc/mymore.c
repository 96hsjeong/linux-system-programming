#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd[2];
	pid_t child;

	if (pipe(fd) == -1) { // pipe 생성 
		perror("pipe");
		exit(1);
	}

	child = fork(); // process 생성 
	if (child == (pid_t)(-1)) {
		perror("fork");
		exit(1);
	}
	else if (child == (pid_t)0) { // Child Process
		close(1); // 1번 파일 디스크립터 close 
		close(fd[0]); // Child read close

		if (dup(fd[1]) == -1) { // fd[1]과 1번을 duplicate
			perror("dup");
			exit(1);
		}

		if ((execlp("ls", "ls", "-l", NULL)) == -1) { // ls -l 실행 
			perror("execlp");
			exit(1);
		}
	}
	else { // Parent Process
		close(0); // 0번 파일 디스크립터 close 
		close(fd[1]); // Parent write close

		if (dup2(fd[0], 0) == -1) { // duplicate 지정, 0번과 duplicate
			perror("dup");
			exit(1);
		}

		if ((execlp("more", "more", NULL)) == -1) { // more 실행 
			perror("execlp");
			exit(1);
		}
	}

	return 0;
}
