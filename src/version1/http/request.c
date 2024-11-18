#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// 서버 주소 및 포트
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 39071

#define BUFFER_SIZE 4096

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char send_buffer[BUFFER_SIZE];
    char recv_buffer[BUFFER_SIZE];

    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
    {
        perror("서버 주소 설정 실패");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("서버 연결 실패");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // HTTP GET 요청 메시지 작성. 수정 필요
    snprintf(send_buffer, sizeof(send_buffer),
             "GET /?docName=example HTTP/1.1\r\n"
             "Host: %s:%d\r\n"
             "\r\n",
             SERVER_IP, SERVER_PORT);

    // 서버로 요청 전송
    if (send(sockfd, send_buffer, strlen(send_buffer), 0) < 0)
    {
        perror("요청 전송 실패");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 서버로부터 응답 수신
    ssize_t received_bytes;
    while ((received_bytes = recv(sockfd, recv_buffer, sizeof(recv_buffer) - 1, 0)) > 0)
    {
        recv_buffer[received_bytes] = '\0'; // 문자열 끝 표시
        printf("%s", recv_buffer);
    }

    if (received_bytes < 0)
    {
        perror("응답 수신 실패");
    }

    // 소켓 닫기
    close(sockfd);
    return 0;
}
