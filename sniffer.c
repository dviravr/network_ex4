#include "pcap.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <net/ethernet.h>


/* Ethernet header */
struct ethheader {
  u_char  ether_dhost[ETHER_ADDR_LEN]; /* destination host address */
  u_char  ether_shost[ETHER_ADDR_LEN]; /* source host address */
  u_short ether_type;                  /* IP? ARP? RARP? etc */
};

/* IP Header */
struct ipheader {
  unsigned char      iph_ihl:4, //IP header length
                     iph_ver:4; //IP version
  unsigned char      iph_tos; //Type of service
  unsigned short int iph_len; //IP Packet length (data + header)
  unsigned short int iph_ident; //Identification
  unsigned short int iph_flag:3, //Fragmentation flags
                     iph_offset:13; //Flags offset
  unsigned char      iph_ttl; //Time to Live
  unsigned char      iph_protocol; //Protocol type
  unsigned short int iph_chksum; //IP datagram checksum
  struct  in_addr    iph_sourceip; //Source IP address 
  struct  in_addr    iph_destip;   //Destination IP address 
  unsigned char icmp_type;
 	unsigned char icmp_code;
};

/* ICMP Header*/
struct icmphdr{
	#define ICMP_ECHO_REQ 8
	#define ICMP_ECHO_RES 0
	#define ICMP_HDR_LEN 4
 	unsigned char icmp_type;
 	unsigned char icmp_code;
 	unsigned short icmp_cksum;		/* icmp checksum */
 	unsigned short icmp_id;				/* icmp identifier */
 	unsigned short icmp_seq;			/* icmp sequence number */
};

void got_packet(u_char *args, const struct pcap_pkthdr *header, 
                              const u_char *packet)
{
  struct ethheader *eth = (struct ethheader *)packet;

  if (ntohs(eth->ether_type) == 0x0800) { // 0x0800 is IP type
    struct ipheader * ip = (struct ipheader *)(packet + sizeof(struct ethheader)); 

   
    /* determine protocol */
   // if(ip->iph_protocol==IPPROTO_ICMP) {                               
       // struct icmphdr *icmp_hdr = (struct icmphdr *)(packet + sizeof(ip->iph_len));
        switch(ip->iph_protocol) {                               
        case IPPROTO_TCP:
            printf("   Protocol: TCP\n");
            return;
        case IPPROTO_UDP:
            printf("   Protocol: UDP\n");
            return;
        case IPPROTO_ICMP:

            printf("From: %s\n", inet_ntoa(ip->iph_sourceip));  
            printf("To: %s\n", inet_ntoa(ip->iph_destip));
            printf("Type: %d\n",(unsigned char)ip->icmp_type );
            printf("Code: %d\n////end of packet////////\n",(unsigned char)ip->icmp_code);
            return;
        default:
            printf("   Protocol: others\n");
            return;
    }
      /*  {
            printf("From: %s\n", inet_ntoa(ip->iph_sourceip));  
            printf("To: %s\n", inet_ntoa(ip->iph_destip));
            printf("Type: %d\n",(unsigned char)icmp_hdr->icmp_type );
            printf("Code: %d\n////end of packet////////\n",(unsigned char)icmp_hdr->icmp_code);

        };*/
        
    //}
  }
}
 
int main()
{
  pcap_t *handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
  char filter_exp[] = "ip proto icmp";
  bpf_u_int32 net;

  // Step 1: Open live pcap session on NIC with name eth3
  handle = pcap_open_live("eth3", BUFSIZ, 1, 1000, errbuf); 

  // Step 2: Compile filter_exp into BPF psuedo-code
  pcap_compile(handle, &fp, filter_exp, 0, net);      
  pcap_setfilter(handle, &fp);                             

  // Step 3: Capture packets
  pcap_loop(handle, -1, got_packet, NULL);                

  pcap_close(handle);   //Close the handle 
    return 0;
}
