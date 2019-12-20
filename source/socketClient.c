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

 // UDP通信---客户端
 int client(){
     // ===========================================
         // 1. 创建socket
             // ===========================================
                 // 服务器端文件描述符
                     int client_fd;
                         client_fd = socket(AF_INET, SOCK_DGRAM, 0);

                             // ===========================================
                                 // 2. 通过struct sockaddr_in
                                     // ===========================================
                                         // 给客户端指定端口
                                             struct sockaddr_in client_addr;
                                                 memset(&client_addr, 0, sizeof(client_addr));
                                                     client_addr.sin_family = AF_INET;
                                                         client_addr.sin_port = htons(PORT);


                                                             int bindRet = bind(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr));
                                                                 if(bindRet == -1){
                                                                         printf("error at %d bind fail!!!\n",(__LINE__ - 2));
                                                                                 return -1;
                                                                                     }

                                                                                         // 
                                                                                             struct sockaddr_in server_addr;

                                                                                                 // 对struct sockaddr_in初始化
                                                                                                     // 经过实验验证sizeof(server_addr) = 16
                                                                                                         memset(&server_addr, 0, sizeof(server_addr));

                                                                                                             server_addr.sin_family = AF_INET;
                                                                                                                 inet_pton(AF_INET, "192.168.50.128", &server_addr.sin_addr);
                                                                                                                     server_addr.sin_port = htons(PORT);

                                                                                                                         // ===========================================
                                                                                                                             // 3. sendto
                                                                                                                                 // ===========================================
                                                                                                                                     // UDP数据包缓存区（包含待发送数据）
                                                                                                                                         char sendBuf[SEND_BUF_LEN] = {'c','l','i','e','n','t','!','!'};
                                                                                                                                             // 发送数据长度
                                                                                                                                                 int sendLen;
                                                                                                                                                     sendLen = sendto(client_fd, sendBuf, sizeof(sendBuf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

                                                                                                                                                         printf("sendLen = %d\n", sendLen);
                                                                                                                                                             // 打印输出内容
                                                                                                                                                                 printf("sendBuf: %s\n", sendBuf);

                                                                                                                                                                     // ===========================================
                                                                                                                                                                         // 4. recvfrom
                                                                                                                                                                             // ===========================================
                                                                                                                                                                                 // 发送数据的客户端地址信息的结构体
                                                                                                                                                                                     struct sockaddr_in send_server_addr;
                                                                                                                                                                                         socklen_t socklen = sizeof(send_server_addr);

                                                                                                                                                                                             // 用于缓存接收缓存
                                                                                                                                                                                                 char recBuf[RECE_BUF_LEN] = {'0'};
                                                                                                                                                                                                     int recLen;
                                                                                                                                                                                                         recLen = recvfrom(client_fd, recBuf, sizeof(recBuf), 0, (struct sockaddr*)&send_server_addr, &socklen);

                                                                                                                                                                                                             printf("recLen = %d\n", recLen);
                                                                                                                                                                                                                 // inet_ntop调用成功时，这个指针就是inet_ntop函数的返回值
                                                                                                                                                                                                                     char str[INET_ADDRSTRLEN] = {'0'};
                                                                                                                                                                                                                         printf("receive from %s at port %d.\n", inet_ntop(AF_INET, &send_server_addr.sin_addr, str, sizeof(str)), ntohs(send_server_addr.sin_port));
                                                                                                                                                                                                                             printf("recBuf = %s\n", recBuf);

                                                                                                                                                                                                                                 // ===========================================
                                                                                                                                                                                                                                     // 6. close
                                                                                                                                                                                                                                         // ===========================================
                                                                                                                                                                                                                                             close(client_fd);
                                                                                                                                                                                                                                                 return 0;
                                                                                                                                                                                                                                                 }

                                                                                                                                                                                                                                                 void main(){
                                                                                                                                                                                                                                                     client();
                                                                                                                                                                                                                                                     }
