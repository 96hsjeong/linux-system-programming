#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main()
{
	DIR *dp;
	struct dirent *dep;

	if ((dp = opendir(".")) == NULL) {
			perror("opendir");
			exit(0);
	}

	while(dep = readdir(dp)) {
		printf("%s\n", dep->d_name);
	}

	closedir(dp);

	return 0;
}


