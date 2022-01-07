#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 1024

long filesize(FILE *fp) // 파일 크기 반환하는 함수
{
    long cur, size;

    cur = ftell(fp); // 파일포인터 현재 위치 cur에 저장
    fseek(fp, 0L, SEEK_END); // 파일포인터 맨끝으로 이동
    size = ftell(fp); // 파일포인터 현재 위치 (파일 맨끝) size에 저장
    fseek(fp, cur, SEEK_SET); // 파일포인터 cur 위치로 이동

    return size; // 파일 크기 반환
}

int main(int argc, char *argv[])
{
    FILE *src, *dst1, *dst2;
    char buf[MAX_BUF];
    int count, size;

    if (argc != 4) { // 필요한 인자가 잘못되면 에러 출력
        fprintf(stderr, "Usage: %s source dst1 dst2\n", argv[0]);
        exit(1);
    }

    if ((src = fopen(argv[1], "rb")) == NULL) { // 소스파일 열기, 바이너리파일 읽기
        perror("fopen");
        exit(1);
    }

    if ((dst1 = fopen(argv[2], "wb")) == NULL) { // 목적파일1 열기, 바이너리파일 쓰기
        perror("fopen");
        exit(1);
    }

    if ((dst2 = fopen(argv[3], "wb")) == NULL) { // 목적파일2 열기, 바이너리파일 쓰기
        perror("fopen");
        exit(1);
    }

    size = filesize(src) / 2; // 소스파일 절반 크기

    while (size > 0) { // 소스파일 처음부터 절반까지 목적파일1에 쓰기
        count = (size > MAX_BUF) ? MAX_BUF : size;
        fread(buf, 1, count, src);
        fwrite(buf, 1, count, dst1);
        size -= count;
    }

    while ((count = fread(buf, 1, MAX_BUF, src)) > 0) { // 소스파일 절반 이후부터 끝까지 목적파일2에 쓰기
        fwrite(buf, 1, count, dst2);
    }

    fclose(src); // 파일 닫기
    fclose(dst1); // 파일 닫기
    fclose(dst2); // 파일 닫기

    return 0;
}


