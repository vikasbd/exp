#include "common.h"
int
add_new_packet (int index, const uint8_t *pkt, uint32_t len)
{
    glinfo.pktinfo[index].len = len;
    memcpy(glinfo.pktinfo[index].pkt, pkt, len);
    // Copy the correct SourceMAC.
    memcpy(glinfo.pktinfo[index].pkt + ETH_ALEN, 
           glinfo.srcmac.ether_addr_octet, ETH_ALEN);
    //memcpy(glinfo.pktinfo[index].pkt, glinfo.dstmac, ETH_ALEN);
    glinfo.num_pkts++;
    return 0;
}

int
read_data_file ()
{
    char                errbuff[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr  *header;
    const uint8_t       *packet;   
    int                 pkt_index = 0;
    pcap_t              *handler = NULL;
    
    handler = pcap_open_offline(glopts.pcapfile, errbuff);

    while (pcap_next_ex(handler, &header, &packet) >= 0) {
        LOG_DEBUG("Packet # %i", pkt_index);
        LOG_DEBUG("Packet size: %d bytes", header->len);
        add_new_packet(pkt_index, packet, header->len);
        pkt_index++;
    }
}

int
init_destination_address ()
{
    uint8_t dest[ETH_ALEN] = {0x08, 0x00, 0x27, 0xbf, 0x95, 0xfd};

    bzero(&glinfo.address, sizeof(glinfo.address));
    glinfo.address.sll_ifindex = glinfo.ifindex;
    glinfo.address.sll_halen = ETH_ALEN;
    memcpy(glinfo.address.sll_addr, dest, ETH_ALEN);
}

int
prepare_message ()
{
    init_destination_address();
    return 0;
}

int
init_sender_iovecs ()
{
    for (int i = 0; i < MAX_NUM_PKTS; i++) {
        glinfo.iovecs[i].iov_len = glinfo.pktinfo[i].len;
    }

    return 0;
}


int
send_multi_packets ()
{
    if (sendmmsg(glinfo.fd, glinfo.mmsgs, glinfo.num_pkts, 0) < 0) {
        perror("sendmmsg");
        exit(1);
    }

    LOG_INFO("Packets sent successfully.");
    return 0;
}
#if 0
int
send_packets ()
{
    struct msghdr       msg;
    struct iovec        msgvec;
    
    msgvec.iov_base = glinfo.pktinfo[0].pkt;
    msgvec.iov_len = glinfo.pktinfo[0].len;

    bzero(&msg, sizeof(msg));
    msg.msg_name = &glinfo.address;
    msg.msg_namelen = sizeof(glinfo.address);
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;

    msg.msg_iov = &msgvec;
    msg.msg_iovlen = 1;

    if (sendmsg(glinfo.fd, &msg, 0) < 0) {
        perror("sendmsg");
        exit(1);
    }

    LOG_INFO("Packet sent successfully.");
    return 0;
}
#endif

int
main (int argc, char *argv[])
{
    int     ret = 0;

    parse_args(argc, argv);

    init_glinfo();

    if (IS_RUNMODE_SENDER()) {
        read_data_file();
        prepare_message();
        init_sender_iovecs();
        send_multi_packets();
    }

    return 0;
}
