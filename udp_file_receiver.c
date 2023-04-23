#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <listen-port> <output-file>\n", argv[0]);
        exit(1);
    }

    int listen_port = atoi(argv[1]);
    char *output_file = argv[2];

    FILE *file = fopen(output_file, "w");
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
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(listen_port);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    char buffer[BUFFER_SIZE];

    // Nhận tên file
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    if (recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("Receiving file: %s\n", buffer);

    // Nhận nội dung file
    while (1) {
        ssize_t bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received == -1) {
            perror("recvfrom");
            exit(1);
        } else if (bytes_received == 0) {
            break;
        }

        fwrite(buffer, 1, bytes_received, file);
    }

    fclose(file);
    close(sock);

    return 0;
}
