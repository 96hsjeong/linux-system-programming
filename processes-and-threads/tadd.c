#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// thread에 다중인자 전달하기위해서 struct 정의
typedef struct _SumArg
{
	int start;
	int end;
} SumArg;

// 인자로 들어온 숫자의 시작부터 끝까지의 합 구하기
void *SumThread(void *arg)
{
	long sum = 0;
	int i;
	SumArg *argv = (SumArg *)arg;
	int start = argv->start;
	int end = argv->end;

	for (i = start; i <= end; i++)
		sum += i;
	
	printf("sum finish: %ld!!!\n", sum);
	
	return (void *)sum;
}

int main()
{
	SumArg arg1 = {1, 50};
	SumArg arg2 = {51, 100};
	pthread_t tid1, tid2;
	int tsum1, tsum2;

	if (pthread_create(&tid1, NULL, (void *)SumThread, (void *)&arg1) < 0) { // 쓰레드1 생성
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)SumThread, (void *)&arg2) < 0) { // 쓰레드2 생성 
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, (void *)&tsum1) < 0) { // 쓰레드1 종료 
		perror("pthread_join");
		exit(1);
	}

	if (pthread_join(tid2, (void *)&tsum2) < 0) { // 쓰레드2 종료 
		perror("pthread_join");
		exit(1);
	}

	printf("SumThread(1~50) + SumThread(51~100) = %d\n", tsum1 + tsum2); // 각 쓰레드에서 구한 합 더해서 출력 

	return 0;
}

