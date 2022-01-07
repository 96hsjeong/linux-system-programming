#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

#define MAX_PATH 256
#define MAX_BUF 16

void printFileInfo(char *path, char *filename) // 파일 정보 출력 함수 
{
	struct stat statbuf;

	struct passwd *pwd;
	struct group *grp;

	struct tm *lctm;
	char timebuf[MAX_BUF];

	char fullpath[MAX_PATH]; // 파일 경로
	strcpy(fullpath, path);
	strcat(fullpath, "/");
	strcat(fullpath, filename);

	if (lstat(fullpath, &statbuf) < 0) {
		perror("lstat");
		return;
	}

	// file type
	if (S_ISREG(statbuf.st_mode))
		printf("%c", '-');
	else if (S_ISDIR(statbuf.st_mode))
		printf("%c", 'd');
	else if (S_ISCHR(statbuf.st_mode))
		printf("%c", 'c');
	else if (S_ISFIFO(statbuf.st_mode))
		printf("%c", 'f');
	else if (S_ISLNK(statbuf.st_mode))
		printf("%c", 'l');
	else if (S_ISSOCK(statbuf.st_mode))
		printf("%c", 's');

	// file mode
	if (S_IRUSR & statbuf.st_mode)
		printf("%c", 'r');
	else
		printf("%c", '-');

	if (S_IWUSR & statbuf.st_mode)
		printf("%c", 'w');
	else
		printf("%c", '-');

	if (S_IXUSR & statbuf.st_mode)
		printf("%c", 'x');
	else
		printf("%c", '-');

	if (S_IRGRP & statbuf.st_mode)
		printf("%c", 'r');
	else
		printf("%c", '-');
		
	if (S_IWGRP & statbuf.st_mode)
		printf("%c", 'w');
	else
		printf("%c", '-');
		
	if (S_IXGRP & statbuf.st_mode)
		printf("%c", 'x');
	else
		printf("%c", '-');
		
	if (S_IROTH & statbuf.st_mode)
		printf("%c", 'r');
	else
		printf("%c", '-');
		
	if (S_IWOTH & statbuf.st_mode)
		printf("%c", 'w');
	else
		printf("%c", '-');
		
	if (S_IXOTH & statbuf.st_mode)
		printf("%c", 'x');
	else
		printf("%c", '-');

	pwd = getpwuid(statbuf.st_uid); // uid로 username 얻기
	grp = getgrgid(statbuf.st_gid); // gid로 groupname 얻기

	lctm = localtime(&statbuf.st_mtime); // 시간형식 바꾸기
	strftime(timebuf, MAX_BUF, "%b %d %H:%M", lctm);
		
	// 파일 정보 출력
	printf(" %ld %s %s %5ld %s %s\n", statbuf.st_nlink, pwd->pw_name, grp->gr_name, statbuf.st_size, timebuf, filename);
}


void printFileList(char *path) // 디렉토리 안의 모든 파일 정보 출력 함수
{
	DIR *dp;
	struct dirent *dep;
	
	if ((dp = opendir(path)) == NULL) {
		perror("opendir");
		return;
	}

	while (dep = readdir(dp)) {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printFileInfo(path, dep->d_name); // 파일 정보 출력
	}

	closedir(dp);
}


void printDirList(char *path, int argc, char *argv[])
{
	struct stat statbuf;
	char fullpath[MAX_PATH];
	int i;

	for (i = 1; i < argc; i++) { // 파일인 경우만 먼저 출력 
		if (lstat(argv[i], &statbuf) < 0) {
			perror("lstat");
			continue;
		}
		if(S_ISDIR(statbuf.st_mode))
			continue;
		printFileInfo(path, argv[i]);
	}
	
	for (i = 1; i < argc; i++) { // 디렉토리인 경우 내부의 모든 파일 출력 
		if (lstat(argv[i], &statbuf) < 0) {
			continue;
		}
		if (S_ISDIR(statbuf.st_mode)) {
			printf("\n%s:\n", argv[i]);
			strcpy(fullpath, "./");
			strcat(fullpath, argv[i]);
			printFileList(fullpath);
		}
	}
}


int main(int argc, char *argv[])
{
	if (argc == 1)  // 인자가 하나만 들어옴 ./mylsl -> 현재 디렉토리의 모든 파일정보 출력
		printFileList(".");
	else if (argc > 1) // 인자가 두개 이상 들어옴 ./mylsl file1 file2 file3 -> 인자로 입력한 파일의 정보 출력, 디렉토리인 경우 내부 파일 정보 출력.
		printDirList(".", argc, argv);

	return 0;
}

