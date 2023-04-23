#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    char find[]="0123456789";
    int server_socket, client_socket, ret,len;
    char buffer[BUFSIZE];
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len;
    int opt=1;


    

    //Tạo socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == -1)
    {
        perror("socket() failed");
        return 1;
    }
    //Thiết lập thông tin địa chỉ server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9000);
    // Liên kết socket với địa chỉ server
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        perror("bind() failed");
        return 1;
    }
    //Lắng nghe kết nối từ client
    if (listen(server_socket, 5))
    {
        perror("listen() failed");
        return 1;
    }
    
    // Chấp nhận kết nối từ client
    client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket < 0) {
         perror("accept() failed");
        return 1;
    }
    // Nhận dữ liệu từ client và đếm số lần xuất hiện của xâu ký tự "0123456789"
    ret = read(client_socket, buffer, BUFSIZE);
    int count = 0;
    len=strlen(find);
     while (strstr(buffer, find) != NULL)
    {
        strcpy(buffer, strstr(buffer, find) + len);
        count++;
        
    }

     
    // In kết quả và đóng kết nối
    printf("Số lần xuất hiện của xâu ký tự '0123456789': %d\n", count);
    close(client_socket);
    close(server_socket);

    return 0;
}


       

