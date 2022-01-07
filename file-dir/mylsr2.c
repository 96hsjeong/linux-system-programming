#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define	MAX_PATH	256

void
JustDoIt(char *path)
{
	DIR				*dp;
	struct dirent	*dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL)  {
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n", path);
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		printf("%s\n", dep->d_name);
	}

	rewinddir(dp);
	while (dep = readdir(dp))  {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name);
		if (lstat(fullpath, &statbuf) < 0)  {
			perror("lstat");
			exit(1);
		}
		if (S_ISDIR(statbuf.st_mode))  {
			JustDoIt(fullpath);
		}
	}
	
	closedir(dp);
}

int main()
{
	JustDoIt(".");
	return 0;
}
