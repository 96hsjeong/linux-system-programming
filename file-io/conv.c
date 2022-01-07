#include <stdio.h>
#include <string.h>
#include "record.h"

int main(int argc, char *argv[])
{
	FILE *src, *dst;
	Record rec;

	if (argc != 3) { // 필요한 인자가 없으면 에러 출력
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL) { // 소스파일 열기, 텍스트파일 읽기
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wb")) == NULL) { // 목적파일 열기, 바이너리파일 쓰기
		perror("fopen");
		exit(1);
	}

	while (fgets(rec.stud, SMAX, src)) { // 소스파일에서 한줄 읽어서 rec.stud에 저장, 소스파일 읽을거 없을때까지  반복
		*strchr(rec.stud, '\n') = '\0'; // 문자열에서 \n을 \0으로 변환
		fgets(rec.num, NMAX, src); // 소스파일에서 한줄 읽어서 rec.num에 저장
		*strchr(rec.num, '\n') = '\0'; // 문자열에서 \n을 \0으로 변환 
		fgets(rec.dept, DMAX, src); // 소스파일에서 한줄 읽어서 rec.dept에 저장 
		*strchr(rec.dept, '\n') = '\0'; // 문자열에서 \n을 \0으로 변환 

		fwrite(&rec, sizeof(rec), 1, dst); // 목적파일에 rec 쓰기
	}

	fclose(src); // 파일 닫기 
	fclose(dst); // 파일 닫기

	return 0;
}
