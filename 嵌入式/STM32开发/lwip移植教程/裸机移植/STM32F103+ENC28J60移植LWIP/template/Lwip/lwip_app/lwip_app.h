#ifndef __LWIP_APP_H__
#define __LWIP_APP_H__

#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "lwip/tcpip.h" 

#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/tcp.h"
#include "netif/etharp.h"
#include "lwip/timeouts.h"
#include "lwip/udp.h"
#include "lwip/pbuf.h"

extern void process_mac(void);
extern void Lwip_init(void);
extern void Lwip_init_task(void *arg);

err_t
ethernetif_init(struct netif *netif);
extern struct netif enc28j60_netif;
extern uint32_t lwip_localtime;
#endif
