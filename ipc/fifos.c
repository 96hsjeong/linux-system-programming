#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include "fifo.h"

// 시그널 들어오면 FIFO 파일 삭제하는 함수 
void SigIntHandler(int signo)
{
	if (remove(SERV_FIFO) < 0) {
		perror("remove");
		exit(1);
	}

	exit(0);
}

int main()
{
	int fd, cfd, n;
	MsgType msg;

	if (signal(SIGINT, SigIntHandler) == SIG_ERR) { // Ctrl+C 시그널 들어오면 SigIntHandler함수 실행
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0) { // FIFO 생성 
		if (errno != EEXIST) {
			perror("mkfifo");
			exit(1);
		}
	}

	if ((fd = open(SERV_FIFO, O_RDWR)) < 0) { // special file FIFO 열기 
		perror("open");
		exit(1);
	}

	while (1) {
		if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0) { // 메시지 기다리기 
			if (errno == EINTR) {
				continue;
			}
			else {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s......", msg.data); // 메시지 출력 

		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) { // returnFifo 열기 
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid()); // 메시지 쓰기 
		write(cfd, (char *)&msg, sizeof(msg)); // 파일에 쓰기
		close(cfd); // 파일 닫기 
		printf("Replied.\n");
	}

	return 0;
}
