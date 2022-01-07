#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CMD 256

// 입력 커맨드를 출력하는 함수 
void DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");

	pthread_exit(NULL); // 쓰레드 종료 
}

int main()
{
	char cmd[MAX_CMD];
	pthread_t tid;

	while(1) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); 
		if (cmd[0] == 'q')
			break;
		if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0) { // 쓰레드 생성
			perror("pthread_create");
			exit(1);
		}

#if 0
		pthread_join(tid_NULL);
#endif
	}

	return 0;
}
