#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip6.h>
#include <netinet/icmp6.h>
#include <net/if.h>

#define ICMP_HDRLEN 8
#define IPV6_HDRLEN 40
#define RT_HDRLEN   20

int main(int argc, const char *argv[])
{
    int sockfd;
    struct sockaddr_in6 dst;
    struct ip6_hdr iphdr;
    struct ip6_rthdr rthdr;
    struct in6_addr sraddr;
    struct icmp6_hdr icmphdr;
    char   payload[10];
    uint8_t *ip_pkt;    // Packet string
    const int on = 1;

    sockfd = socket(AF_INET6, SOCK_RAW, IPPROTO_RAW);
    if(sockfd < 0)
        perror("socket create");

    // argv[1] src addr; argv[2] dst addr
    dst.sin6_family = AF_INET6;
    inet_pton(AF_INET6, argv[1], &dst.sin6_addr);


    // ipv6 header
    iphdr.ip6_flow = htonl((6 << 28) | (0 << 20) | 0);
    iphdr.ip6_plen = htons(0);
    iphdr.ip6_nxt = IPPROTO_ROUTING;
    iphdr.ip6_hops = 64;
    iphdr.ip6_plen = htons(88);
    inet_pton(AF_INET6, argv[1], &iphdr.ip6_dst);
    inet_pton(AF_INET6, "::1", &iphdr.ip6_src);
    // routing header
    rthdr.ip6r_nxt = IPPROTO_ICMPV6;	
    rthdr.ip6r_len = 23;	
    rthdr.ip6r_type = 0;
    rthdr.ip6r_segleft = 1;
    inet_pton(AF_INET6, "::2", &sraddr);
    // icmpv6 header
    icmphdr.icmp6_type = ICMP6_ECHO_REQUEST;
    icmphdr.icmp6_code = 0;
    icmphdr.icmp6_id = 0x2345;
    icmphdr.icmp6_seq = htons(0x01);
    icmphdr.icmp6_cksum = 0x1234;
    strcpy(payload, "helloyifan");
    if (setsockopt (sockfd, IPPROTO_IPV6, IP_HDRINCL, &on, sizeof (on)) < 0) {
        perror ("setsockopt() failed to set IP_HDRINCL ");
        exit (EXIT_FAILURE);
    }
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", "lo");

    // Bind socket to interface index.
    if (setsockopt (sockfd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof (ifr)) < 0) {
        perror ("setsockopt() failed to bind to interface ");
        exit (EXIT_FAILURE);
    }
    ip_pkt = (uint8_t *)malloc(sizeof(uint8_t)*(IPV6_HDRLEN + 38));
    if(ip_pkt < 0)
        perror("ipv6 packet create");
    memcpy(ip_pkt, &iphdr, sizeof(iphdr));
    memcpy(ip_pkt + IPV6_HDRLEN, &rthdr, sizeof(rthdr));
    memcpy(ip_pkt + IPV6_HDRLEN + RT_HDRLEN, &icmphdr, sizeof(icmphdr));
    memcpy(ip_pkt + IPV6_HDRLEN + RT_HDRLEN + ICMP_HDRLEN, &payload, sizeof(payload));
    int bytes = sendto(sockfd, ip_pkt, 78, 0, (struct sockaddr *)&dst, sizeof(dst));
    if(bytes < 0)
        perror("send failed");
    return 1;
}
