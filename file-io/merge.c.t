#include <stdio.h>

#define MAX_BUF 1024

int main(int argc, char *argv[])
{
    FILE *src1, *src2, *dst;
    char buf[MAX_BUF];
    int count;

    if (argc != 4) { // 필요한 인자가 잘못되면 에러 출력
        fprintf(stderr, "Usage: %s src1 src2 dst\n", argv[0]);
        exit(1);
    }

    if ((src1 = fopen(argv[1], "rb")) == NULL) { // 소스파일1 열기, 바이너리파일 읽기
        perror("fopen");
        exit(1);
    }

    if ((src2 = fopen(argv[2], "rb")) == NULL) { // 소스파일2 열기, 바이너리파일 읽기
        perror("fopen");
        exit(1);
    }

    if ((dst = fopen(argv[3], "wb")) == NULL) { // 목적파일 열기, 바이너리파일 쓰기
        perror("fopen");
        exit(1);
    }

    while ((count = fread(buf, 1, MAX_BUF, src1)) > 0) { // 소스파일1을 목적파일에 쓰기
        fwrite(buf, 1, count, dst);
    }

    while ((count = fread(buf, 1, MAX_BUF, src2)) > 0) { // 소스파일2를 목적파일에 쓰기
        fwrite(buf, 1, count, dst);
    }

    fclose(src1); // 파일 닫기
    fclose(src2); // 파일 닫기
    fclose(dst); // 파일 닫기

    return 0;
}
