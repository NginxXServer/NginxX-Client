#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CMD_BUFFER_SIZE 2048 // 명령어 길이를 위한 버퍼 크기
#define LOG_DIR "log"        // 로그 디렉토리 (기존에 존재한다고 가정)

// 사용자로부터 로그 파일 이름을 입력받는 함수
void get_log_filename(char *filename, size_t size)
{
    printf("로그 파일 이름을 입력하세요 (기본값: siege_result.txt): ");
    if (fgets(filename, size, stdin) != NULL)
    {
        // 개행 문자 제거
        filename[strcspn(filename, "\n")] = '\0';
        if (strlen(filename) == 0)
        {
            // 입력이 비어 있으면 기본값 사용
            strncpy(filename, "siege_result.txt", size);
        }
    }
    else
    {
        perror("Failed to read input");
        exit(EXIT_FAILURE);
    }
}

// siege 명령어 실행 함수
void run_siege(const char *command, const char *log_filename)
{
    FILE *fp;
    char buffer[128];

    // 로그 디렉토리 경로와 파일명을 결합하여 전체 경로를 만든다.
    char log_file_path[CMD_BUFFER_SIZE];
    snprintf(log_file_path, sizeof(log_file_path), "%s/%s", LOG_DIR, log_filename);

    // 파일 포인터 생성하여 로그 파일 열기
    fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("Failed to execute siege command");
        exit(EXIT_FAILURE);
    }

    // 명령어 출력 결과를 파일로 저장
    FILE *log_file = fopen(log_file_path, "w");
    if (log_file == NULL)
    {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    // siege 명령어의 출력을 읽어와서 로그 파일에 기록
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        fputs(buffer, log_file);
    }

    // 파일 닫기
    fclose(log_file);
    pclose(fp);
    printf("Siege results saved to: %s\n", log_file_path);
}

int main()
{
    // 사용자로부터 로그 파일 이름 받기
    char log_filename[256];
    get_log_filename(log_filename, sizeof(log_filename));

    // siege 명령어 구성
    const char *siege_command = "siege -v -c 255 -t 1m -f urls.txt";

    // siege 명령어 실행 및 결과 저장
    run_siege(siege_command, log_filename);

    return 0;
}
