#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<ctype.h>
#include<arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include<pthread.h>


// =================================================
// =================================================
//                  宏定义
// =================================================
// =================================================
#define RECE_BUF_LEN 128 // receive buffer length
#define SEND_BUF_LEN 128 // send buffer length
#define PORT_1      0xa5a6 // port
#define PORT_2      0xa5a7 // port
#define PORT_3      0xa5a6 // port
#define SERVER_INIT_IP_1      "192.168.50.130"
#define SERVER_INIT_IP_2      "192.168.50.130"

#define CLIENT_INIT_IP      "192.168.50.129"

typedef struct {
    int* server_fd;
    int domain;
    int type;
    int protocol;
    struct sockaddr_in* server_addr;
    const char *strptr;
    uint16_t hostshort;
    int serverIndex;
}SERVER_INFO;

// =================================================
// =================================================
//                  全局变量
// =================================================
// =================================================
int g_uDebugFlag = 0;

// 服务器端文件描述符
int server_fd_1;
int server_fd_2;

// 发送数据的客户端地址信息的结构体
struct sockaddr_in client_addr_from;
socklen_t socklen = sizeof(client_addr_from);

// 用于存储接收到的数据
char receBuf[RECE_BUF_LEN];

// 用于存储发送的数据
char sendBuf[SEND_BUF_LEN];


struct sockaddr_in client_addr;
// sockaddr数据结构struct sockaddr_in中包含了IP地址协议类型、IP地址和端口号
struct sockaddr_in server_addr_1;
struct sockaddr_in server_addr_2;

// =================================================
// =================================================
//                  函数
// =================================================
// =================================================
// 对struct sockaddr_in进行设置
int setSockaddr_in(struct sockaddr_in *pAddr, sa_family_t sin_family, const char *strptr, uint16_t hostshort){
    // 对socket数据结构内存初始化
    memset(pAddr, 0, sizeof(struct sockaddr_in));

    // printf("sizeof(struct sockaddr_in) = %d\n", sizeof(struct sockaddr_in));
    // printf("sizeof(pAddr) = %d\n", sizeof(pAddr));
    // printf("sizeof(*pAddr) = %d\n", sizeof(*pAddr));

    if(g_uDebugFlag){
        printf("%s at %d : hostlong = %s\n", __FUNCTION__, __LINE__, strptr);
    }

    (*pAddr).sin_family = sin_family;
    // 两种配置绑定IP的方式
    // 方式一
    inet_pton(sin_family, strptr, &(*pAddr).sin_addr.s_addr);
    // 方式二
    
    (*pAddr).sin_port = htons(hostshort);
    // (*pAddr).sin_port = htons(PORT); // 这里写死，统一使用同一个端口号

    // =============================
    // 打印查看一下
    // =============================
    // inet_ntop调用成功时，这个指针就是inet_ntop函数的返回值
    char str[INET_ADDRSTRLEN] = {0};
    printf("set struct sockaddr_in IP: %s; port %d.\n", inet_ntop(AF_INET, &(*pAddr).sin_addr, str, sizeof(str)), ntohs((*pAddr).sin_port));
    // printf("str = %s\n", str);

    return 0;
}

int getSockaddr_in(struct sockaddr_in* pAddr){
    // inet_ntop调用成功时，这个指针就是inet_ntop函数的返回值
    char str[INET_ADDRSTRLEN] = {0};
    printf("get struct sockaddr_in IP: %s; port %d.\n", inet_ntop(AF_INET, &(*pAddr).sin_addr, str, sizeof(str)), ntohs((*pAddr).sin_port));
    // printf("str = %s\n", str);

    return 0;
}
// 默认初始化服务端
int serverInit(int* server_fd, int domain, int type, int protocol, struct sockaddr_in* server_addr, const char *strptr, uint16_t hostshort){
    // ==========================================================
    // 1. 创建socket
    // ==========================================================
    if((*server_fd = socket(domain, type, protocol)) == -1){
        printf("%s at %d : server socket created fail !!!\n", __FUNCTION__,__LINE__);
        return -1;
    }
    // ==========================================================
    // 2. 通过struct sockaddr_in 结构体设置服务器地址和监听端口
    // ==========================================================
    if(setSockaddr_in(server_addr, domain, strptr, hostshort) != 0){
        printf("%s at %d : setSockaddr_in set fail !!!\n", __FUNCTION__,__LINE__);
        return -1;
    }

    // ==========================================================
    // 3. 使用bind()函数绑定监听端口，将套接字文件描述符和地址类型
    //    变量（struct sockaddr_in）进行绑定
    // ==========================================================
    if(bind(*server_fd, (struct sockaddr*)server_addr, sizeof(struct sockaddr_in)) == -1){
        printf("%s at %d : bind fail !!!\n", __FUNCTION__,__LINE__);
        return -1;
    }
    return 0;
}

int serverSend(char* sendBuf, int sendBufLen, int serverIndex, int* server_fd){
    printf("=================== server_%d begin to send message =====================\n", serverIndex);
    // ==========================================================
    // 5. 向客户端发送数据，是用sendto()函数向服务器主机发送数据
    // ==========================================================

    // ==========================================================
    // 这里需要添加一个判断，用于选择发给谁
    // ==========================================================

    int sendLen = 0;
    sendLen = sendto(*server_fd, sendBuf, sendBufLen, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    
    // inet_ntop调用成功时，这个指针就是inet_ntop函数的返回值
    char str[INET_ADDRSTRLEN] = {0};
    printf("send to %s at port %d.\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)), ntohs(client_addr.sin_port));
    // printf("str = %s\n", str);

    printf("sendLen = %d\n", sendLen);
    if(sendLen <= 0){
        printf("%s at %d : sendLen  0 fail !!!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    // =========================================================
    // =========================================================
    // 打印发送内容
    // =========================================================
    // =========================================================
    printf("sendBuf: %s\n", sendBuf);
    printf("=================== server_%d now send message over =====================\n", serverIndex);
    printf("\n");

    // =========================================================
    // =========================================================
    // 发送结束之后，需要把发送缓存清空
    // 如果不清空，如果再下次接收的个数少于之前的内容，
    // 那么再次发送的时候，会把上次保留的也发出来
    // =========================================================
    // =========================================================
    memset(sendBuf, 0, sendBufLen);
    // ==========================================================
    // 6. 关闭套接字，是用close()函数释放资源
    // 如果在这里关闭套接字，那么，server_fd就被释放了，又因为server_fd
    // 是全局变量，server_fd释放后，就不再是socket了，recfrom就不会再
    // 阻塞了，就导致了无限循环
    // ==========================================================
    // close(server_fd);
    return 0;
}

int serverReceive(int* sockfd, int domain, int serverIndex){
    // ==========================================================
    // 4. 接收客户端的数据，使用recvfrom()函数接收客户端的网络数据
    // ==========================================================
    while(1){
        printf("===================serverReceive_%d begin to receive message =====================\n", serverIndex);
        int receLen;
        receLen = recvfrom(*sockfd, receBuf, sizeof(receBuf), 0, (struct sockaddr*)&client_addr_from, &socklen);
        printf("receLen = %d\n", receLen);

        // inet_ntop调用成功时，这个指针就是inet_ntop函数的返回值
        char str[INET_ADDRSTRLEN] = {0};
        printf("receive from %s at port %d.\n", inet_ntop(domain, &client_addr_from.sin_addr, str, sizeof(str)), ntohs(client_addr_from.sin_port));
        // printf("str = %s\n", str);

        if(receLen <= 0){
            printf("%s at %d : receLen  0 fail !!!\n", __FUNCTION__,__LINE__);
            continue;
        }

        // =========================================================
        // =========================================================
        // 打印接收内容
        // =========================================================
        // =========================================================
        printf("receBuf = %s\n", receBuf);
        printf("===================serverReceive_%d now receive message over =====================\n", serverIndex);
        printf("\n");

        // if(setSockaddr_in(&client_addr, AF_INET, inet_ntop(AF_INET, &client_addr_from.sin_addr, str, sizeof(str)), ntohs(client_addr_from.sin_port)) != 0){
        //     printf("%s at %d : setSockaddr_in set fail !!!\n", __FUNCTION__,__LINE__);
        //     continue;
        // }

        // 指定发给特定客户端
        if(setSockaddr_in(&client_addr, domain, CLIENT_INIT_IP, PORT_3) != 0){
            printf("%s at %d : setSockaddr_in set fail !!!\n", __FUNCTION__,__LINE__);
            continue;
        }

        // ===================================================
        // ===================================================
        // 注意：如果不serverSend(receBuf，需要在这里
        //       把接收buf清空，不然输出信息有误。
        // memset(receBuf, 0, receLen);
        // ===================================================
        // ===================================================
        if(serverSend(receBuf, receLen, serverIndex, sockfd) != 0){
            printf("%s at %d : server_%d serverSend fail !!!\n", __FUNCTION__, __LINE__, serverIndex);
            continue;
        }
    }
    // ==========================================================
    // 5. 关闭套接字，是用close()函数释放资源
    // ==========================================================
    close(*sockfd);
    return 0;
}

void* server(void* args){
    SERVER_INFO* info = (SERVER_INFO*) args;
    // 主要完成创建socket、初始化设置服务器地址和监听端口、将套接字
    // socket id, 协议类，协议名，协议，struct sockaddr_in*，IP，端口号绑定监听端口
    if(serverInit(info->server_fd, info->domain, info->type, info->protocol, info->server_addr, info->strptr, info->hostshort) != 0){
        printf("%s at %d : server_%d serverInit fail !!!\n", __FUNCTION__, __LINE__, info->serverIndex);
        return (void*)-1;
    }

    if(getSockaddr_in(info->server_addr) != 0){
        printf("%s at %d : server_%d getSockaddr_in server_addr fail !!!\n", __FUNCTION__, __LINE__, info->serverIndex);
        return (void*)-1;
    }

    if(serverReceive(info->server_fd, info->domain, info->serverIndex) != 0){
        printf("%s at %d : server_%d serverReceive fail !!!\n", __FUNCTION__, __LINE__, info->serverIndex);
        return (void*)-1;
    }
    return (void*)-1;
}

void main(){
        SERVER_INFO server_1 = {
        &server_fd_1,
        AF_INET,
        SOCK_DGRAM,
        0,
        &server_addr_1,
        CLIENT_INIT_IP,
        PORT_3,
        0,
    };

    server(&server_1);
}

// void main(){
//     SERVER_INFO server_1 = {
//         &server_fd_1,
//         AF_INET,
//         SOCK_DGRAM,
//         0,
//         &server_addr_1,
//         SERVER_INIT_IP_1,
//         PORT_1,
//         1,
//     };

//     SERVER_INFO server_2 = {
//         &server_fd_2,
//         AF_INET,
//         SOCK_DGRAM,
//         0,
//         &server_addr_2,
//         SERVER_INIT_IP_2,
//         PORT_2,
//         2,
//     };

//     // 表示th是一条新的线程，此时还没有创建线程，
//     // 只是定义了一个变量，如同int a，还有没给a复制。
//     pthread_t th1;
//     pthread_create(&th1, NULL, server, &server_1);
//     pthread_t th2;
//     pthread_create(&th2, NULL, server, &server_2);

//     // 注意，这里th不需要带&
//     pthread_join(th1, NULL);
//     pthread_join(th2, NULL);
// }
