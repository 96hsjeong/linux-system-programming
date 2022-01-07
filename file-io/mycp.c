#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUF 1024 // 버퍼 최대 크기

int main(int argc, char *argv[])
{
	int fd1, fd2, count;
	char buf[MAX_BUF];

	if (argc != 3) { // 필요한 인자가 잘못되면 에러 출력
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fd1 = open(argv[1], O_RDONLY)) < 0) { // 파일을 읽기모드로 열기
		perror("open fd");
		exit(1);
	}

	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) { // 파일을 쓰기모드로 열고, 파일이 없으면 만들기, 기존 데이터 제거, rw/r/r
		perror("open fd");
		exit(1);
	}

	while ((count = read(fd1, buf, MAX_BUF)) > 0) { // 파일 다 읽을때까지 반복, 버퍼에 버퍼 크기만큼 파일 읽기
		write(fd2, buf, count); // 파일에 버퍼를 쓰기
	}

	close(fd1); // fd1 close
	close(fd2); // fd2 close

	return 0;
}
