#include <stdio.h>
#include <unistd.h>

#define MAX_CMD 256

// 입력 커맨드를 출력하는 함수 
void DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

int main()
{
	char cmd[MAX_CMD];

	while (1) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); // 문자열 입력 
		if (cmd[0] == 'q') // 반복 중단 
			break;
		
		DoCmd(cmd); // 함수 실행 
	}

	return 0;
}

