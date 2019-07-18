//#include "lwip_comm.h" 
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h" 
#include "lwip/timeouts.h"
//#include "lwip/tcp_impl.h"
//#include "lwip/ip_frag.h"
#include "lwip/tcpip.h" 
//#include "malloc.h"
//#include "delay.h"
//#include "usart.h"  
#include <stdio.h>
#include "ucos_ii.h"

OS_STK * TCPIP_THREAD_TASK_STK;	

