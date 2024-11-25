#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CMD_BUFFER_SIZE 2048 // 버퍼 크기 증가
#define LOG_DIR "wrk/wrk_log"
#define SCRIPT_DIR "wrk/wrk_script"

// 디폴트 값 정의
#define DEFAULT_THREADS "1"
#define DEFAULT_CONNECTIONS "10"
#define DEFAULT_DURATION "10s"

// 로그 파일 이름 생성 함수
void generate_log_filename(char *filename, size_t size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    snprintf(filename, size, "%s/wrk_log_%04d-%02d-%02d_%02d-%02d-%02d.txt",
             LOG_DIR,
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
}

// Lua 스크립트 파일이 존재하는지 확인하는 함수
int file_exists(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// wrk 명령 실행 및 로그 저장
void execute_wrk(const char *wrk_command, const char *log_filename)
{
    FILE *fp;
    char buffer[128];

    // 로그 파일 오픈
    FILE *log_file = fopen(log_filename, "w");
    if (log_file == NULL)
    {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    // wrk 명령 실행
    fp = popen(wrk_command, "r");
    if (fp == NULL)
    {
        perror("Failed to execute wrk command");
        fclose(log_file);
        exit(EXIT_FAILURE);
    }

    // wrk 출력 읽어서 로그 파일에 쓰기
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        fputs(buffer, log_file);
    }

    // 파일 닫기
    if (pclose(fp) == -1)
    {
        perror("Failed to close wrk process");
    }
    fclose(log_file);
}

void get_user_input(const char *prompt, char *buffer, size_t size, const char *default_value)
{
    printf("%s (기본값: %s): ", prompt, default_value);
    if (fgets(buffer, size, stdin) != NULL)
    {
        // 개행 문자 제거
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) == 0)
        {
            // 입력이 비어 있으면 기본값 사용
            strncpy(buffer, default_value, size);
        }
    }
    else
    {
        perror("Failed to read input");
        exit(EXIT_FAILURE);
    }
}

// URL과 Lua 스크립트 파일명을 파라미터로 받아 wrk 실행
int run_wrk(const char *url)
{
    if (url == NULL || strlen(url) == 0)
    {
        fprintf(stderr, "Error: URL is required.\n");
        return EXIT_FAILURE;
    }

    char lua_script[256];
    char full_command[CMD_BUFFER_SIZE];
    char log_filename[256];
    char threads[16], connections[16], duration[16];
    int cmd_length;

    // 사용자 입력 받기
    get_user_input("스레드 개수 입력", threads, sizeof(threads), DEFAULT_THREADS);
    get_user_input("연결 개수 입력", connections, sizeof(connections), DEFAULT_CONNECTIONS);
    get_user_input("테스트 지속 시간 입력 (예: 10s, 1m)", duration, sizeof(duration), DEFAULT_DURATION);

    // Lua 스크립트 파일명 입력 받기
    get_user_input("사용할 Lua 스크립트 파일명 입력 (없으면 Enter)", lua_script, sizeof(lua_script), "");

    // Lua 스크립트 파일이 입력되었으면 존재하는지 확인
    if (strlen(lua_script) > 0)
    {
        char lua_script_path[512];
        snprintf(lua_script_path, sizeof(lua_script_path), "%s/%s", SCRIPT_DIR, lua_script);

        if (!file_exists(lua_script_path))
        {
            fprintf(stderr, "Error: Lua script file '%s' does not exist in '%s'.\n", lua_script, SCRIPT_DIR);
            return EXIT_FAILURE;
        }

        // Lua 스크립트가 존재하는 경우, 명령어 생성
        cmd_length = snprintf(full_command, sizeof(full_command), "wrk -t%s -c%s -d%s -s %s %s", threads, connections, duration, lua_script_path, url);
    }
    else
    {
        // Lua 스크립트 없이 기본 wrk 명령
        cmd_length = snprintf(full_command, sizeof(full_command), "wrk -t%s -c%s -d%s %s", threads, connections, duration, url);
    }

    // 명령어 길이 확인
    if (cmd_length >= sizeof(full_command))
    {
        fprintf(stderr, "Error: Command too long, exceeds buffer size of %zu bytes\n", sizeof(full_command));
        return EXIT_FAILURE;
    }

    // 로그 파일 이름 생성
    generate_log_filename(log_filename, sizeof(log_filename));

    printf("Executing command: %s\n", full_command);
    printf("Saving log to: %s\n", log_filename);

    // wrk 실행 및 로그 저장
    execute_wrk(full_command, log_filename);

    printf("wrk results saved to %s\n", log_filename);

    return EXIT_SUCCESS;
}
