// 사용 예 : $./upper file
// 파일의 모든 문자를 대문자로 변경하여 출력
#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int ch;

    if (argc != 2) { // 필요한 인자가 잘못되면 에러 출력
        printf("Usage: %s filename \n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "rt")) == NULL) { // file open 에러 출력
        perror("fopen");
        exit(1);
    }

    // 대문자로 출력 
    while ((ch = fgetc(fp)) != EOF) { // 파일의 끝까지 문자 단위로 읽으면서 반복
        if (ch >= 'a' && ch <= 'z') // 소문자인 경우
            printf("%c", ch-32); // 대문자와 소문자의 ASCII Code 차이가 32이므로 'ch-32'하여 대문자로 출력
        else // 소문자가 아닌 경우
            printf("%c", ch); // 그냥 출력
    }

    fclose(fp); // file close

    return 0;
}
