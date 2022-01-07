#include <stdio.h>
#include <unistd.h>
#include "record.h"

void print_record(Record *rp) // Record 출력 함수
{
    puts(rp->stud);
    puts(rp->num);
    puts(rp->dept);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    Record rec;

    if (argc != 2) { // 필요한 인자가 없으면 에러
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    if ((fp = fopen(argv[1], "r+b")) == NULL) { // 파일 열기, 바이너리파일 기존파일 날리지 않고 쓰기
        perror("fopen");
        exit(1);
    }

    printf("-----Ordered Record List-----\n");  

    while (fread(&rec, sizeof(rec), 1, fp)) { // 파일에서 Record 단위로 읽기 
        print_record(&rec); // Record 출력
    }
    rewind(fp); // 파일포인터 맨앞으로 이동
    getchar(); // 문자 입력

    printf("-----Shuffled Record List-----\n");
    fseek(fp, sizeof(rec)*2L, SEEK_SET); // 파일 포인터 세번째 Record로 가리킴 (파일 시작부터 두개 Record 이동)
    fread(&rec, sizeof(rec), 1, fp); // Record 한개 읽기
    print_record(&rec); // Record 출력

    fseek(fp, sizeof(rec)*5L, SEEK_SET); // 파일 포인터 여섯번째 Record로 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 한개 읽기
    print_record(&rec); // Record 출력

    fseek(fp, sizeof(rec)*1L, SEEK_SET); // 파일 포인터 두번째 Record로 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 한개 읽기
    print_record(&rec); // Record 출력

    fseek(fp, sizeof(rec)*3L, SEEK_SET); // 파일 포인터 네번재 Record 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 한개 읽기
    print_record(&rec); // Record 출력

    fseek(fp, sizeof(rec)*0L, SEEK_SET); // 파일 포인터 첫번째 Record 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 한개 읽기
    print_record(&rec); // Record 출력

    fseek(fp, sizeof(rec)*4L, SEEK_SET); // 파일 포인터 다섯번째 Record 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 한개 읽기
    print_record(&rec); // Record 출력
    getchar(); // 문자입력

    printf("-----Updated Record List-----\n");

    fseek(fp, sizeof(rec)*2L, SEEK_SET); // 파일 포인터 세번째 Record 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 읽기
    strcpy(rec.dept, "Movie"); // Record에서 dept를 "Movie"로 변경
    fseek(fp, sizeof(rec)*2L, SEEK_SET); // 파일 포인터 세번째 Record 가리킴
    fwrite(&rec, sizeof(rec), 1, fp); // Record 쓰기

    fseek(fp, sizeof(rec)*5L, SEEK_SET); // 파일포인터 여섯번째 Record 가리킴
    fread(&rec, sizeof(rec), 1, fp); // Record 읽기
    strcpy(rec.dept, "Baseball"); // Record에서 dept를 "Baseball"로 변경
    fseek(fp, sizeof(rec)*5L, SEEK_SET); // 파일포인터 여섯번때 Record 가리킴
    fwrite(&rec, sizeof(rec), 1, fp); // Record 쓰기

    rewind(fp); // 파일포인터 맨앞으로 이동
    while (fread(&rec, sizeof(rec), 1, fp))  { // 파일에서 Record 단위로 읽기
        print_record(&rec); // Record 출력
    }

    fclose(fp); // 파일 닫기 

    return 0;
}
