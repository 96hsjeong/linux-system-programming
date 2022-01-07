#include <stdio.h>

#define MAX_BUF 256

int main(int argc, char *argv[])
{
    FILE *src, *dst;
    char buf[MAX_BUF];

    if (argc != 3) { // 필요한 인자가 잘못되면 에러 출력  
        fprintf(stderr, "Usage: %s source destination\n", argv[0]);
        exit(1);
    }

    if ((src = fopen(argv[1], "rt")) == NULL) { // 첫번째 파일 열기, 텍스트 읽기
        perror("fopen");
        exit(1);
    }

    if ((dst = fopen(argv[2], "wt")) == NULL) { // 두번째 파일 열기, 텍스트 쓰기
        perror("fopen");
        exit(1);
    }

    while (fgets(buf, MAX_BUF, src) != NULL) { // 줄 단위로 파일 읽기
        fputs(buf, dst); // 목적지 파일에 줄단위 쓰기
    }

    fclose(src); // 파일 닫기 
    fclose(dst); // 파일 닫기

    return 0;
}

