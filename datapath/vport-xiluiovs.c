/*
 * Copyright (c) Xilinx, Inc.
 * VVDN
*/

#include "datapath.h"
#include "gso.h"
#include "vport.h"
#include "vport-internal_dev.h"
#include "vport-netdev.h"
#include <linux/module.h>

static void xiluiovs_destroy(struct vport *vport);
static int xiluiovs_send(struct sk_buff *skb);
static struct vport *xiluiovs_vport_create(const struct vport_parms *parms);

static struct vport_ops ovs_xiluiovs_vport_ops = {
        .type           = OVS_VPORT_TYPE_XILUIOVS,
        .create         = xiluiovs_vport_create,
        .destroy        = xiluiovs_destroy,
        .send           = xiluiovs_send,
};


static int xiluiovs_dev_change_mtu(struct net_device *netdev, int new_mtu)
{
	
	printk("vvdn debug:  func : %s line : %u mtu : %d\n",__func__,__LINE__,new_mtu);

        if (new_mtu < 68)
                return -EINVAL;

        netdev->mtu = new_mtu;
        return 0;
}

static const struct net_device_ops xiluiovs_dev_netdev_ops = {
	//TODO
	.ndo_change_mtu = xiluiovs_dev_change_mtu,
};

//do_setup

static void xiluiovs_netdev_setup(struct net_device *netdev)
{
        netdev->netdev_ops = &xiluiovs_dev_netdev_ops;
}

static struct vport *xiluiovs_vport_create(const struct vport_parms *parms)
{
	struct vport *vport;

	printk("vvdn debug:  func : %s line : %u parms->name : %s\n",__func__,__LINE__,parms->name);
	
	vport = ovs_vport_alloc(0,&ovs_xiluiovs_vport_ops,parms);

	if(IS_ERR(vport)) {
	printk("vvdn debug:  func : %s line : %u vport error and vport : %p\n",__func__,__LINE__,vport);
		return vport;
	}

	printk("vvdn debug:  func : %s line : %u vport : %p\n",__func__,__LINE__,vport);

	vport->dp = parms->dp;

	vport->dev = alloc_netdev(0,parms->name, NET_NAME_UNKNOWN, xiluiovs_netdev_setup);

	vport->dev->mtu = 1500;

	return vport;
}

static void xiluiovs_destroy(struct vport *vport)
{

}

static int xiluiovs_send(struct sk_buff *skb)
{
	printk("vvdn debug : func : %s line : %u sending packet %d bytes\n",__func__,__LINE__,skb->len);

	kfree_skb(skb);

	return 0;
}

 
int __init xiluiovs_init(void)
{
        return ovs_vport_ops_register(&ovs_xiluiovs_vport_ops);
}

void xiluiovs_exit(void)
{
        ovs_vport_ops_unregister(&ovs_xiluiovs_vport_ops);
}

module_init(xiluiovs_init);
module_exit(xiluiovs_exit);

MODULE_DESCRIPTION("OVS: XILUIOVS switching port");
MODULE_LICENSE("GPL");
