#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int host_order() 
{
    unsigned long a = 0x12345678; // 十进制305 419 896
    unsigned char * p = (unsigned char *)(&a);

    // %0x和%x都是以十六进制格式右对齐输出，输出的是无符号数。
    printf("主机字节序:%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]);

    unsigned long b = htonl(a);

    p = (unsigned char *)(&b);

    printf("网络字节序:%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]);

    return 0;
}

int net_order()
{
    struct in_addr ipaddr;
    unsigned long addr = inet_addr("192.168.1.100");
    unsigned char *p = (unsigned char *)(&addr);
    printf("addr = %u \n",addr);
    printf("%0x %0x %0x %0x : inet_addr 192.168.1.100\n",p[0],p[1],p[2],p[3]);
    printf("%d  %d  %d  %d : inet_addr 192.168.1.100\n",p[0],p[1],p[2],p[3]);
    unsigned long addr_host = ntohl(addr);
    p = (unsigned char *)(&addr_host);
    printf("%0x %0x %0x %0x : inet_addr ntohl 192.168.1.100\n",p[0],p[1],p[2],p[3]);
    printf("%d  %d  %d  %d : inet_addr ntohl 192.168.1.100\n",p[0],p[1],p[2],p[3]);
    printf("addr_host = %u\n",addr_host);
    return 0;
}

int main()
{
    int ret = 0;
    ret = host_order();
    printf("====================================\n");
    ret = net_order();
    return ret ;
}