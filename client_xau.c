#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sockfd;
    char buffer[256];
    FILE *fp;
    int n;


    // Tạo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    // Tạo địa chỉ kết nối của server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9000);

    // Kết nối đến server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    // Đọc nội dung file văn bản
    fp = fopen("xau.txt", "r");
    if (fp == NULL)
        perror("Không thể mở file");

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Gửi dữ liệu đến server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
            perror("Không thể gửi dữ liệu đến server");
    }


   
    // Đóng kết nối
    close(sockfd);
    fclose(fp);
    return 0;
}