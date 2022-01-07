#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	char ch = '\0';

	if ((fd = creat("file.hole", 0400)) < 0) { // file.hole 파일 생성, r/-/-
		perror("creat");
		exit(1);
	}

	if (lseek(fd, 1023, SEEK_SET) < 0) { // 파일포인터를 파일의 맨앞에서 1023바이트 떨어진 지점으로 이동
		perror("lseek");
		exit(1);
	}

	write(fd, &ch, 1); // 파일에 '\0' (1바이트) 쓰기

	close(fd); // 파일 닫기

	return 0;
}

