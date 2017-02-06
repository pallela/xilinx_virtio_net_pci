/*
 * Copyright (c) 2014, 2015 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <config.h>

#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <config.h>
#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dirs.h"
#include "dp-packet.h"
#include "dpif-netdev.h"
#include "fatal-signal.h"
#include "list.h"
#include "netdev-provider.h"
#include "netdev-vport.h"
#include "odp-util.h"
#include "ofp-print.h"
#include "ovs-numa.h"
#include "ovs-thread.h"
#include "ovs-rcu.h"
#include "packets.h"
#include "shash.h"
#include "sset.h"
#include "unaligned.h"
#include "timeval.h"
#include "unixctl.h"
#include "openvswitch/vlog.h"
#include "vvprintf.h"

VLOG_DEFINE_THIS_MODULE(xiluiovs);

struct netdev * xiluiovs_alloc(void);
int xiluiovs_construct(struct netdev *);
int xiluiovs_destruct(struct netdev *);
int xiluiovs_dealloc(struct netdev *);
int xiluiovs_set_multiq(struct netdev *netdev, unsigned int n_txq,unsigned int r_txq);
int xiluiovs_send(struct netdev *netdev, int qid, struct dp_packet **buffers,int cnt, bool may_steal);
int xiluiovs_set_ethernet_addr(struct netdev *netdev, const struct eth_addr mac);
int xiluiovs_get_ethernet_addr(const struct netdev *netdev, struct eth_addr *mac);
int xiluiovs_get_mtu(const struct netdev *netdev, int *mtup);
int xiluiovs_set_mtu(const struct netdev *netdev, int mtu);
int xiluiovs_get_stats(const struct netdev *netdev, struct netdev_stats *);
int xiluiovs_get_queue(const struct netdev *netdev, unsigned int queue_id, struct smap *details);
int xiluiovs_dump_queue_stats(const struct netdev *netdev, void (*cb)(unsigned int queue_id, struct netdev_queue_stats *, void *aux), void *aux);
int xiluiovs_update_flags(struct netdev *netdev, enum netdev_flags off, enum netdev_flags on, enum netdev_flags *old_flags);
struct netdev_rxq xiluiovs_rxq_alloc(void);
int xiluiovs_rxq_construct(struct netdev_rxq *);
void xiluiovs_rxq_destruct(struct netdev_rxq *);
void xiluiovs_rxq_dealloc(struct netdev_rxq *);
int xiluiovs_rxq_recv(struct netdev_rxq *rx, struct dp_packet **pkts, int *cnt);
void xiluiovs_rxq_wait(struct netdev_rxq *rx);
int xiluiovs_rxq_drain(struct netdev_rxq *rx);



static const struct netdev_class OVS_UNUSED xiluiovs_user_class = {
	"xiluiovs", /* type */
	NULL,      /* init */
	NULL,      /* run */
	NULL,     /* wait */
	xiluiovs_alloc,  /* alloc */
	xiluiovs_construct, /* construct */
	xiluiovs_destruct, /* destruct */
	xiluiovs_dealloc, /* dealloc */
	NULL,  /* get_config */
	NULL, /* set_config */
	NULL, /* get_tunnel_config */
	NULL, /* build_header */
	NULL, /* push_header */
	NULL, /* pop_header */
	NULL, /* get_numa_id */
	xiluiovs_set_multiq, /* set_multiq */
	xiluiovs_send,  /* send */
	NULL,  /* send_wait */
	xiluiovs_set_ethernet_addr, /* set_etheraddr */
	xiluiovs_get_ethernet_addr, /* get_etheraddr */
	xiluiovs_get_mtu, /* get_mtu */
	xiluiovs_set_mtu, /* set_mtu */
	NULL, /* get_if_index */
	NULL, /* get_carrier */
	NULL, /* get_carrier_resets */
	NULL, /* set_miimon_interval */
	xiluiovs_get_stats, /* get_stats */
	NULL, /* get_features */
	NULL, /* set_advertisement */
	NULL, /* set_policing */
	NULL, /* get_qos_types */
	NULL, /* get_qos_capabilities */
	NULL, /* get_qos */
	NULL, /* set_qos */
	xiluiovs_get_queue, /* get_queue */
	NULL, /* set_queue */
	NULL, /* delete_queue */
	NULL, /* get_queue_stats */
	NULL, /* queue_dump_start */
	NULL, /* queue_dump_next */
	NULL, /* queue_dump_done */
	xiluiovs_dump_queue_stats, /* dump_queue_stats */
	NULL, /* get_in4 */
	NULL, /* set_in4 */
	NULL, /* get_in6 */
	NULL, /* add_router */
	NULL, /* get_next_hop */
	NULL, /* get_status */
	NULL, /* arp_lookup */
	xiluiovs_update_flags, /* update_flags */
	xiluiovs_rxq_alloc, /* rxq_alloc */
	xiluiovs_rxq_construct, /* rxq_construct */
	xiluiovs_rxq_destruct, /* rxq_destruct */
	xiluiovs_rxq_dealloc, /* rxq_dealloc */
	xiluiovs_rxq_recv, /* rxq_recv */
	xiluiovs_rxq_wait, /* rxq_wait */
	xiluiovs_rxq_drain, /* rxq_drain */
};

struct netdev * xiluiovs_alloc(void)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	struct netdev *netdev;

	netdev = malloc(sizeof(struct netdev));

	return netdev;
	//return NULL;
}

int xiluiovs_construct(struct netdev *netdev)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_destruct(struct netdev *netdev)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_dealloc(struct netdev *netdev)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	free(netdev);
	return 0;
}

int xiluiovs_set_multiq(struct netdev *netdev, unsigned int n_txq,unsigned int r_txq)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_send(struct netdev *netdev, int qid, struct dp_packet **buffers,int cnt, bool may_steal)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_set_ethernet_addr(struct netdev *netdev, const struct eth_addr mac)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

static unsigned char testaddr[6] = {0x00,0x00,0x00,0x00,0x00,0x01};
//static unsigned char testaddr[6] = {0x00,0x11,0x22,0x33,0x44,0x55};

int xiluiovs_get_ethernet_addr(const struct netdev *netdev, struct eth_addr *mac)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	memcpy(mac,testaddr,6);
	vvprintf("mac address is %02x:%02x:%02x:%02x:%02x:%02x\n",
	mac->ea[0],
	mac->ea[1],
	mac->ea[2],
	mac->ea[3],
	mac->ea[4],
	mac->ea[5]);

	return 0;
}

int xiluiovs_get_mtu(const struct netdev *netdev, int *mtup)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	*mtup = 1500;
	return 0;
}

int xiluiovs_set_mtu(const struct netdev *netdev, int mtu)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_get_stats(const struct netdev *netdev, struct netdev_stats *stats)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_get_queue(const struct netdev *netdev, unsigned int queue_id, struct smap *details)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_dump_queue_stats(const struct netdev *netdev, void (*cb)(unsigned int queue_id, struct netdev_queue_stats *, void *aux), void *aux)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

int xiluiovs_update_flags(struct netdev *netdev, enum netdev_flags off, enum netdev_flags on, enum netdev_flags *old_flags)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	vvprintf("vvdn debug : off = %d on  :%d *old_flags : %d\n",on,off,*old_flags);
	return 0;
}

struct netdev_rxq xiluiovs_rxq_alloc(void)
{
	struct netdev_rxq temp;
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return temp;
}

int xiluiovs_rxq_construct(struct netdev_rxq *rx)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

void xiluiovs_rxq_destruct(struct netdev_rxq *rx)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	
}

void xiluiovs_rxq_dealloc(struct netdev_rxq *rx)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	
}


int xiluiovs_rxq_recv(struct netdev_rxq *rx, struct dp_packet **pkts, int *cnt)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

void xiluiovs_rxq_wait(struct netdev_rxq *rx)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);

}

int xiluiovs_rxq_drain(struct netdev_rxq *rx)
{
	vvprintf("vvdn debug : func : %s line : %u\n",__func__,__LINE__);
	return 0;
}

void netdev_xiluiovs_register(void)
{
	netdev_register_provider(&xiluiovs_user_class);
}
