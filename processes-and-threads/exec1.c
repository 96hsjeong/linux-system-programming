#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	char *argv[] = {"mycp", "exec.c", "exec2.c", NULL};

	// execl(char *path, char *arg)
	if ((pid = fork()) < 0) { // 프로세스 생성
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		// 다른 프로그램 실행
		if (execl("/home/lsp41/hw03/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0) {
			perror("execl");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0) { // child 프로세스 종료까지 대기
		perror("waitpid");
		exit(1);
	}

	// execv(char *path, char *argv[])
	if ((pid = fork()) < 0) { // 프로세스 생성
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		// 다른 프로그램 실행
		if (execv("/home/lsp41/hw03/mycp", argv) < 0) {
			perror("execv");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0) { // child 프로세스 종료까지 대기 
		perror("waitpid"); 
		exit(1);
	}

	// execlp(char *file, char *arg)
	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		// PATH에 등록된 디렉토리에 있는 프로그램 실행
		if (execlp("env", "env", NULL) < 0) {
			perror("execlp");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0) { // child 프로세스 종료까지 대기
		perror("waitpid");
		exit(1);
	}

	return 0;
}

