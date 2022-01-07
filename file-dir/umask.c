#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{	
	umask(0); // mask를 하나도 하지 않음
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // bar 파일 생성 rw-/rw-/rw-
		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // mask 켜라 (g:rw, o:rw)
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) < 0) { // foo 파일 생성 rw-/---/---
		perror("creat");
		exit(1);
	}

	return 0;
}
