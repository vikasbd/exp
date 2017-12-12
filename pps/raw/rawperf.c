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
    int i = 0;
    int pktindex = 0;

    if (glinfo.num_pkts < MAX_NUM_PKTS) {
        // If we have less than 1K packets, fill the remaining iovecs,
        // with same packets.
        LOG_DEBUG("NumPkts:%d < MaxPkts:%d", glinfo.num_pkts, MAX_NUM_PKTS);
        for (i = glinfo.num_pkts; i < MAX_NUM_PKTS; i++) {
            glinfo.pktinfo[i].len = glinfo.pktinfo[pktindex].len;
            memcpy(glinfo.pktinfo[i].pkt, glinfo.pktinfo[pktindex].pkt,
                   MAX_PKT_SIZE);
            pktindex = (pktindex + 1) % glinfo.num_pkts;
        }
        glinfo.num_pkts = MAX_NUM_PKTS;
        LOG_DEBUG("Updating NumPkts to %d", glinfo.num_pkts);
    }

    for (i = 0; i < MAX_NUM_PKTS; i++) {
        glinfo.iovecs[i].iov_len = glinfo.pktinfo[i].len;
    }

    return 0;
}


int
start_sender ()
{
    read_data_file();
    prepare_message();
    init_sender_iovecs();

    while (1) {
        if (sendmmsg(glinfo.fd, glinfo.mmsgs, glinfo.num_pkts, 0) < 0) {
            perror("sendmmsg");
            exit(1);
        }
        LOG_DEBUG("Packets sent successfully.");
    }

    return 0;
}

int
start_receiver ()
{
    int count = 0;
    uint8_t buffer[4096];

    while (1) {
        bzero(buffer, sizeof(buffer));
        //count = recvmmsg(glinfo.fd, &glinfo.mmsgs[0], MAX_NUM_PKTS, 0, NULL);
        count = recv(glinfo.fd, buffer, 1500, 0);
        //count = recvfrom(glinfo.fd, buffer, 1500, 0, NULL, NULL);
        if (count < 0) {
            perror("recvmmsg");
            exit(1);
        }

        LOG_INFO("Received %d packets.", count);
        continue;
        for (int i = 0; i < count; i++) {
            struct mmsghdr *msg = &glinfo.mmsgs[i];
            int len = msg->msg_len;
            msg->msg_hdr.msg_flags = 0;
            msg->msg_len = 0;
            LOG_INFO(" - Packet#%d: Length:%d", i, len);
        }
    }

    LOG_INFO("Packets sent successfully.");
    return 0;
}

int
main (int argc, char *argv[])
{
    int     ret = 0;

    parse_args(argc, argv);

    init_glinfo();

    if (IS_RUNMODE_SENDER()) {
        start_sender();
    } else if (IS_RUNMODE_RECEIVER()) {
        start_receiver();
    } else {
        LOG_ERROR("Sender or Receiver not specified.");
    }

    return 0;
}
