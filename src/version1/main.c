#include <string.h>

extern int run_client(int target_port, char *target_host);

int main()
{
    int target_port = 39071; // 프록시 서버 포트
    // char *target_host = "10.198.138.212"; // 프록시 서버 주소(swist1)
    char *target_host = "10.198.138.213"; // 프록시 서버 주소(swist2)

    // char *target_host = "127.0.0.1"; // 로컬 테스트용

    return run_client(target_port, target_host);
}