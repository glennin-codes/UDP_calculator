#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 8080
#define MAXLINE 1024

int calculate(int num1, int num2, char operator)
{
    switch (operator) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0)
                return num1 / num2;
            else
                return 0;
        default:
            return 0;
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);

    } 


    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    
            printf("server started waiting for the client for connections...\n");
    
   
    
    int len, n;
    char buffer[MAXLINE];
    char* message = "Hello from server";
    while (1) {
        len = sizeof(cliaddr); //len is value/result
        n = recvfrom(sockfd, (char*)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr*)&cliaddr,
                     &len);
        int num1 = atoi(strtok(buffer, " "));
        int num2 = atoi(strtok(NULL, " "));
        char operator = strtok(NULL, " ")[0];

        int result = calculate(num1, num2, operator);

        memset(buffer, 0, MAXLINE);
        sprintf(buffer, "%d", result);
        sendto(sockfd, (const char*)buffer, strlen(buffer),
               MSG_CONFIRM, (const struct sockaddr*)&cliaddr,
               len);
    }

    close(sockfd);
    return 0;
}