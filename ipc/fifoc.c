#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "fifo.h"

int main()
{
	char fname[MAX_FIFO_NAME];
	int fd, sfd, n;
	MsgType msg;

	sprintf(fname, ".fifo%d", getpid()); // client specific한 fifo 만들기 
	if (mkfifo(fname, 0600) < 0) { // fifo 생성 
		perror("mkfifo");
		exit(1);
	}

	if ((fd = open(fname, O_RDWR)) < 0) { // 나만 사용할 FIFO 파일 읽기/쓰기 전용으로 열기 
		perror("open");
		exit(1);
	}
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) { // 서버에 데이터 보낼 FIFO 파일 읽기/쓰기 전용으로 열기 
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d.", getpid()); // 메시지 저장 
	write(sfd, (char *)&msg, sizeof(msg)); // 메시지 쓰기 
	printf("Sent a request......");

	if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0) { // 메시지 기다리기 
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd); // 파일 닫기 
	close(sfd); // 파일 닫기 

	if (remove(fname) < 0) { // FIFO 삭제 
		perror("remove");
		exit(1);
	}

	return 0;
}
