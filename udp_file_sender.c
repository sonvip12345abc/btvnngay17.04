#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <receiver-ip> <receiver-port> <file-to-send>\n", argv[0]);
        exit(1);
    }

    char *receiver_ip = argv[1];
    int receiver_port = atoi(argv[2]);
    char *file_name = argv[3];

    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(receiver_ip);
    server_addr.sin_port = htons(receiver_port);

    char buffer[BUFFER_SIZE];

    // Gửi tên file
    strncpy(buffer, file_name, BUFFER_SIZE);
    if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        exit(1);
    }

    printf("Sending file: %s\n", file_name);

    // Gửi nội dung file
    while (1) {
        size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, file);
        if (bytes_read == 0) {
            break;
        }

        if (sendto(sock, buffer, bytes_read, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("sendto");
            exit(1);
        }
    }


    if (sendto(sock, NULL, 0, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        exit(1);
    }

    fclose(file);
    close(sock);

    return 0;
}