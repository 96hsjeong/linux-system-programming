#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fdin, fdout;
	char *src, *dst;
	struct stat statbuf;

	if (argc != 3) { // 인자가 3개 들어오지않으면 에러출력
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0) { // 읽기전용 파일 열기 
		perror("open");
		exit(1);
	}
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) { // 읽기/쓰기전용 파일 열기 (없으면 생성, 있으면 모두 지우고 생성)
		perror("open");
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0) { // 파일 정보 읽기
		perror("fstat");
		exit(1);
	}
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0) { // 커서를 파일 맨 앞에서부터 파일크기-1 만큼 이동 (파일 끝으로 이동)
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1); // null 쓰기 

	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t) - 1) { // 메모리 맵핑 
		perror("mmap");
		exit(1);
	}
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t) - 1) { // 메모리 맵핑 
		perror("mmap");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size); // src가 가리키는 곳부터 src 크기만큼 dst가 가리키는 곳에 복사

	close(fdin); // 파일 닫기 
	close(fdout); // 파일 닫기 

	return 0;
}
