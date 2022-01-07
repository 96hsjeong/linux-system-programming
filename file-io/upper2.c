#include <stdio.h>

#define MAX_BUF 1024

int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[MAX_BUF];
	int i;

	if (argc != 2) {
		printf("Usage: %s filename \n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	while (fgets(buf, MAX_BUF, fp)) {
		i = 0;
		while (buf[i] != '\0') {
			if (buf[i] >= 'a' && buf[i] <= 'z') 
				printf("%c", buf[i] - 32);
			else
				printf("%c", buf[i]);
			i++;
		}
	}

	fclose(fp);

	return 0;
}
