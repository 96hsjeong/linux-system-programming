#include <stdio.h>

enum { FALSE, TRUE };

void conv(char *fname) // tab을 '    '으로 변경하는 함수
{
    FILE *src, *dst;
    char fdest[40];
    int ch, first;

    if ((src = fopen(fname, "rt")) == NULL) { // 소스파일 읽기, 텍스트파일 읽기
        perror("fopen");
        return;
    }

    strcpy(fdest, fname); // 파일 이름 복사 
    strcat(fdest, ".t"); // 문자열 이어붙이기 (file.t) 
    if ((dst = fopen(fdest, "wt")) == NULL) { // 목적파일 열기, 텍스트파일 읽기
        perror("fopen");
        return;
    }

    first = TRUE;
    while ((ch = fgetc(src)) != EOF) { // 문자 단위로 파일 읽기, 파일의 끝이면 반복 중단
        if (first && ch == '\t') { // 줄의 첫번째 tab이면 공백네개 쓰기
            fputc(' ', dst);
            fputc(' ', dst);
            fputc(' ', dst);
            fputc(' ', dst);
        }
        else {
            fputc(ch, dst); // 문자 단위로 그대로 쓰기
            if (first) 
                first = FALSE; // 줄의 시작이 아니다
            if (ch == '\n') 
                first = TRUE; // 줄바꿈을해서 줄의 시작이다 
        }
    }
    fclose(src); // 파일 닫기
    fclose(dst); // 파일 닫기
}

int main(int argc, char *argv[])
{
    while (--argc) { // 인자로 넣은 파일수만큼 반복 $./tab file1 file2 file3 인 경우 3번 반복
        conv(argv[argc]); // 파일 변환
    }

    return 0;
}

