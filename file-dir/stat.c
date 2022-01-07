#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	struct stat statbuf;
	char *mode;
	int i;

	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		if (lstat(argv[i], &statbuf) < 0) {
			perror("lstat");
			continue;
		}
		if (S_ISREG(statbuf.st_mode)) // regular file 이면
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))  // directory file 이면
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode)) // character special file 이면
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))  // block special file 이면
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode)) // pipe or FIFO 이면
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode)) // symbolic link 이면
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode)) // socker 이면
			mode = "socker";
		printf("%s \n", mode); // file type
		printf("\tst_mode = %d\n", statbuf.st_mode); // file type
		printf("\tst_ino = %d\n", statbuf.st_ino); // i-node number
		printf("\tst_dev = %d\n", statbuf.st_dev); // device number
		printf("\tst_rdev = %d\n", statbuf.st_rdev); // device number for special files
		printf("\tst_nlink = %d\n", statbuf.st_nlink); //number of links
		printf("\tst_uid = %d\n", statbuf.st_uid); // user ID
		printf("\tst_gid = %d\n", statbuf.st_gid); // group ID
		printf("\tst_size = %d\n", statbuf.st_size); // size in bytes
		printf("\tst_atime = %d\n", statbuf.st_atime); // time of last access
		printf("\tst_mtime = %d\n", statbuf.st_mtime); // time of last modification
		printf("\tst_ctime = %d\n", statbuf.st_ctime); // time of last file status change
		printf("\tst_blksize = %d\n", statbuf.st_blksize); // best I/O block size
		printf("\tst_blocks = %d\n", statbuf.st_blocks); // no. of 512-byte blocks allocated
	}

	return 0;
}

