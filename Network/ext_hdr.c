#include <arpa/inet.h>
#include <stdio.h>
#include <error.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>

struct icmp
{
    struct icmp6_hdr icmphdr;
    char   payload[10];
};

int main(int argc, const char *argv[])
{
    int sockfd;
    struct icmp icmppkt;
    struct sockaddr_in6 addr;
    struct ip6_rthdr rthdr;
    struct ip6_hdr iphdr;

    addr.sin6_family = AF_INET6;
    addr.sin6_port = 0;
    inet_pton(AF_INET6, argv[1], &addr.sin6_addr);

    sockfd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    if(sockfd < 0){
        perror("create socket");
        return -1;
    }
    
    rthdr.ip6r_nxt = IPPROTO_ICMPV6;
    rthdr.ip6r_len = 40;
    rthdr.ip6r_type = 2; // RH2
    rthdr.ip6r_segleft = 1;
     
    icmppkt.icmphdr.icmp6_type = ICMP6_ECHO_REQUEST;
    icmppkt.icmphdr.icmp6_code = 0;
    icmppkt.icmphdr.icmp6_id = htons(10000);
    icmppkt.icmphdr.icmp6_seq = htons(0);
    icmppkt.icmphdr.icmp6_cksum = 0;
    strcpy(icmppkt.payload, "hello");
    int res = setsockopt(sockfd, IPPROTO_IPV6, IPV6_RTHDR, &rthdr, sizeof(rthdr));
    if(res < 0)
        perror("setsockopt");
    //icmphdr.icmp6_cksum = checksum(iphdr, icmphdr, data, dalalen);
    int bytes = sendto(sockfd, &icmppkt, sizeof(icmppkt), 0, (struct sockaddr *)&addr, sizeof(addr));
    if(bytes < 0)
    {
        perror("send failed");
    }
    return 0;
}