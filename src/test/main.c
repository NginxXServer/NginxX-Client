#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CMD_BUFFER_SIZE 8192 // 넉넉한 버퍼 크기
#define LOG_DIR "log"        // 로그 디렉토리 (기존에 존재한다고 가정)

// siege 명령 실행 및 로그 파일 저장 함수
void run_siege(const char *command, const char *log_filename)
{
    char log_file_path[CMD_BUFFER_SIZE];
    char full_command[CMD_BUFFER_SIZE];

    // 로그 디렉토리 경로와 파일명을 결합하여 전체 경로를 만든다.
    if (snprintf(log_file_path, sizeof(log_file_path), "%s/%s", LOG_DIR, log_filename) >= sizeof(log_file_path))
    {
        fprintf(stderr, "Log file path is too long!\n");
        exit(EXIT_FAILURE);
    }

    // siege 명령어에 리다이렉션 추가
    if (snprintf(full_command, sizeof(full_command), "%s > %s 2>&1", command, log_file_path) >= sizeof(full_command))
    {
        fprintf(stderr, "Command string is too long!\n");
        exit(EXIT_FAILURE);
    }

    // 명령어 실행
    int ret_code = system(full_command);
    if (ret_code != 0)
    {
        fprintf(stderr, "Failed to execute siege command. Exit code: %d\n", ret_code);
        exit(EXIT_FAILURE);
    }

    printf("Siege results saved to: %s\n", log_file_path);
}

int main()
{
    // 사용자 입력 변수
    int connections;
    char duration[16];

    // 사용자로부터 커넥션 수 입력
    printf("Enter the number of connections: ");
    if (scanf("%d", &connections) != 1 || connections <= 0)
    {
        fprintf(stderr, "Invalid number of connections!\n");
        exit(EXIT_FAILURE);
    }

    // 사용자로부터 실행 시간 입력
    printf("Enter the duration (e.g., 3m, 5s): ");
    if (scanf("%15s", duration) != 1 || strlen(duration) == 0)
    {
        fprintf(stderr, "Invalid duration!\n");
        exit(EXIT_FAILURE);
    }

    // 버퍼 플러시
    while (getchar() != '\n')
        ;

    // 로그 파일 이름 입력
    char log_filename[256];
    printf("Enter the log file name (without extension): ");
    if (fgets(log_filename, sizeof(log_filename), stdin) == NULL)
    {
        fprintf(stderr, "Error reading log file name!\n");
        exit(EXIT_FAILURE);
    }

    // 줄바꿈 제거 (fgets 사용 시 발생 가능)
    log_filename[strcspn(log_filename, "\n")] = '\0';

    if (strlen(log_filename) == 0)
    {
        fprintf(stderr, "Log file name cannot be empty!\n");
        exit(EXIT_FAILURE);
    }

    // 확장자 ".txt" 자동 추가
    if (!strstr(log_filename, ".txt"))
    {
        if (snprintf(log_filename + strlen(log_filename), sizeof(log_filename) - strlen(log_filename), ".txt") >= sizeof(log_filename))
        {
            fprintf(stderr, "Log file name is too long!\n");
            exit(EXIT_FAILURE);
        }
    }

    // siege 명령어 동적 생성
    char siege_command[CMD_BUFFER_SIZE];
    if (snprintf(siege_command, sizeof(siege_command), "siege -R siege.conf -c %d -t %s -f urls.txt", connections, duration) >= sizeof(siege_command))
    {
        fprintf(stderr, "Siege command is too long!\n");
        exit(EXIT_FAILURE);
    }
    printf("\nSiege command: %s\n\n", siege_command);

    // siege 명령어 실행 및 결과 저장
    run_siege(siege_command, log_filename);

    return 0;
}
