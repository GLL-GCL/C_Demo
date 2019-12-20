#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<arpa/inet.h>

// =================================================
// =================================================
//                  宏定义
// =================================================
// =================================================
#define RECE_BUF_LEN 128 // receive buffer length
#define SEND_BUF_LEN 128 // send buffer length
#define PORT      0xa5a6 // port

// =================================================
// =================================================
//                  全局变量
// =================================================
// =================================================


// =================================================
// =================================================
//                  函数
// =================================================
// =================================================

// UDP通信---服务器端
int server(){
    // ===========================================
    // 1. 创建socket
    // ===========================================
    // 服务器端文件描述符
    int server_fd;
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    // ===========================================
    // 2. 通过struct sockaddr_in
    // ===========================================
    // 
    struct sockaddr_in server_addr;

    // 对struct sockaddr_in初始化
    // 经过实验验证sizeof(server_addr) = 16
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // ===========================================
    // 3. bind
    // ===========================================
    int bindRet = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(bindRet == -1){
        printf("error at %d bind fail!!!\n",(__LINE__ - 2));
        return -1;
    }

    // ===========================================
    // 4. recvfrom
    // ===========================================
    // 发送数据的客户端地址信息的结构体
    struct sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);

    // 用于缓存接收缓存
    char recBuf[RECE_BUF_LEN] = {'0'};
    int recLen;
    recLen = recvfrom(server_fd, recBuf, sizeof(recBuf), 0, (struct sockaddr*)&client_addr, &socklen);

    printf("recLen = %d\n", recLen);
    // inet_ntop调用成功时，这个指针就是inet_ntop函数的返回值
    char str[INET_ADDRSTRLEN] = {0};
    printf("receive from %s at port %d.\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)), ntohs(client_addr.sin_port));
    printf("recBuf = %s\n", recBuf);

    // ===========================================
    // 5. sendto
    // ===========================================
    // UDP数据包缓存区（包含待发送数据）
    char sendBuf[SEND_BUF_LEN] = {'s','e','r','v','e','r','!','!','!'};
    // 发送数据长度
    int sendLen;
    sendLen = sendto(server_fd, sendBuf, sizeof(sendBuf), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

    printf("sendLen = %d\n", sendLen);
    printf("sendBuf: %s\n", sendBuf);

    // ===========================================
    // 6. close
    // ===========================================
    close(server_fd);
    return 0;
}

void main(){
    server();
}
