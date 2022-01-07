#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH 256

void JustDoIt(char *path)
{
	DIR *dp;
	struct dirent *dep;
	struct stat statbuf;
	char fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL) { // 디렉토리 열기
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n", path); // 경로 출력
	while(dep = readdir(dp)) { // 디렉토리 안에 파일 읽기
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) // .or ..은 건너뛰기
			continue;
		printf("%s\n", dep->d_name); // 파일 이름 출력
	}

	rewinddir(dp); // 디렉토리 포인터 맨 앞으로

	while (dep = readdir(dp)) { // 디렉토리 안에 파일 읽기
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0) // . or .. 은 건너뛰기
			continue;
		strcpy(fullpath, path); // 경로 복사
		strcat(fullpath, "/"); // / 이어붙이기
		strcat(fullpath, dep->d_name); // 파일이름 이어붙이기
		if (lstat(fullpath, &statbuf) < 0) { // 해당 파일 타입을 statbuf에 저장
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode)) { // 해당 파일 타입이 디렉토리이면 JustDoIt 함수 실행 (재귀)
			JustDoIt(fullpath);
		}
	}

	closedir(dp); // 디렉토리 닫기
}

int main()
{
	JustDoIt(".");
	return 0;
}


