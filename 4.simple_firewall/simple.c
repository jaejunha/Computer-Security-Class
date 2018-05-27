#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/inet.h>

/* This is the structure we shall use to register our function */
static struct nf_hook_ops nfho;

/* This is the hook function itself */
unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int(*okfn)(struct sk_buff *)) {
	struct iphdr *ip;
	struct tcphdr *tcp;

	int nip[4];
	int port;

	ip = ip_hdr(skb);
	tcp = (void *)ip + ip->ihl * 4;

	nip[0] = ((unsigned char *)&ip->saddr)[0];
	nip[1] = ((unsigned char *)&ip->saddr)[1];
	nip[2] = ((unsigned char *)&ip->saddr)[2];
	nip[3] = ((unsigned char *)&ip->saddr)[3];
	port = ntohs(tcp->dest);

	if (port == 23) {
		printk("Block Telnet(from %d.%d.%d.%d:%d)\n", nip[0], nip[1], nip[2], nip[3], port);
		return NF_DROP;
	}
	else if (port == 22) {
		printk("Block SSH(from %d.%d.%d.%d:%d)\n", nip[0], nip[1], nip[2], nip[3], port);
		return NF_DROP;
	}
	else if (ip->saddr == in_aton("157.240.15.38")) {
		printk("Block Facebook(to %d.%d.%d.%d:%d)\n", nip[0], nip[1], nip[2], nip[3], port);
		return NF_DROP;
	}
	else if (ip->saddr == in_aton("104.74.192.17") || (nip[0] == 23 && nip[1] == 35 && nip[2] == 221)) {
		printk("Block Naver(to %d.%d.%d.%d:%d)\n", nip[0], nip[1], nip[2], nip[3], port);
		return NF_DROP;
	}
	else if (ip->saddr == in_aton("192.168.137.126") && ip->protocol == IPPROTO_ICMP) {
		printk("Block ICMP(from %d.%d.%d.%d:%d)\n", nip[0], nip[1], nip[2], nip[3], port);
		return NF_DROP;
	}
	else
		return NF_ACCEPT;
}

/* Initialization routine */
int init_module() {

	/* Fill in our hook structure */
	nfho.hook = hook_func;

	/* Handler function */
	nfho.hooknum = NF_INET_PRE_ROUTING;

	/* First hook for IPv4 */
	nfho.pf = PF_INET;
	nfho.priority = NF_IP_PRI_FIRST;

	/* Make our function first */
	nf_register_net_hook(&init_net, &nfho);

	return 0;
}

/* Cleanup routine */
void cleanup_module() {
	nf_unregister_net_hook(&init_net, &nfho);
}