#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/time.h> // gettimeofday()

int main(int argc, char const *argv[])
{
    struct sockaddr_in s_addr;
    unsigned int s_addr_size = sizeof(s_addr); 
    int sock;
    char packet[IP_MAXPACKET];
    size_t data_size = 0;
    if ((sock = socket (AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) 
    {
        printf ("To create a raw socket, the process needs to be run by Admin/root user.\n\n");
        return -1;
    }
    while (1)
    {
        data_size = recvfrom(sock, packet, IP_MAXPACKET, 0, &s_addr, s_addr_size);
        puts(packet);
    }
    

    return 0;
}
