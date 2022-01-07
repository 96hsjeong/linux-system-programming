#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_BUF 32

// 프로그램 실행 함수 
void myexecv(char *path, char **argv)
{
	pid_t pid;

	if ((pid = fork()) < 0) { // 프로세스 생성 
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {
		if (execv(path, argv) < 0) { // 다른 프로그램 실행 
			perror("execv");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0) { // 프로세스 대기 
		perror("waitpid");
		exit(1);
	}
}

// 명령어 띄어쓰기 단위로 자르기 
void mysystem_cmd(char *c)
{
	char cmd[MAX_BUF];
	strcpy(cmd, c); // 포인터형 문자열을 배열형으로 저장 
	char path[MAX_BUF] = "/bin/"; // 프로그램 경로
	char *argv[MAX_BUF] = {NULL, }; // 명령어 수행 인자 
	int i = 0;
	char *ptr;
	ptr = strtok(cmd, " "); // 띄어쓰기 단위로 자르기 
	while (ptr != NULL)
	{
		argv[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}	

	strcat(path, argv[0]);
	myexecv(path, argv); // 프로그램 실행 함수
}

// 명령어 여러개 입력시 ';' 단위로 자르기 
void mysystem(char *cmd)
{
	char s[MAX_BUF]; 
	strcpy(s, cmd); // 포인터형 문자열을 배열형으로 저장 
	char *sArray[MAX_BUF] = {NULL, }; // ';' 단위로 자른 문자열 저장 
	int i = 0;
	char *ptr1;
	ptr1 = strtok(s, ";");
	while (ptr1 != NULL)
	{
		sArray[i] = ptr1;
		i++;
		ptr1 = strtok(NULL, ";");
	}

	for (i = 0; sArray[i] != NULL; i++)
		mysystem_cmd(sArray[i]); // ; 단위로 자른 명령어 실행 
}

int main() 
{
	mysystem("date");
	mysystem("ls -al");
	mysystem("ps; pwd");

	return 0;
}
