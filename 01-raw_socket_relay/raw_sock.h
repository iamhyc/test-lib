
#ifndef __RAW_SOCK_H__
#define __RAW_SOCK_H__

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define GET_DESTP(iph)  (__be16 *)((char *)iph + iph->ihl*4 + 2)
#define GET_DEST(iph)   ntohs(*GET_DESTP(iph))

#endif //__RAW_SOCK_H__
