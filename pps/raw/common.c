#include "common.h"

global_info_t       glinfo;
global_options_t    glopts;

uint8_t *
mac2str (mac_addr_t mac)
{
    static uint8_t  macstr[64];
    snprintf(macstr, 64, "%02x:%02x:%02x:%02x:%02x:%02x",
             mac.ether_addr_octet[0], mac.ether_addr_octet[1],
             mac.ether_addr_octet[2], mac.ether_addr_octet[3],
             mac.ether_addr_octet[4], mac.ether_addr_octet[5]);
    return macstr;
}

int
print_usage ()
{
    printf("Usage: ./sender [-d] [-v] -i <ifname> -f <pcapfile>\n");
}

int
print_help ()
{
    print_usage();
    printf("Options:\n");
    printf(" -d             : Enable Debugs\n");
    printf(" -v             : Enable Verbose\n");
    printf(" -i <interface> : Interface to send/recv packets.\n");
    printf(" -f <pcapfile>  : PCAP file to be used for packets. (Sender Only)\n");
    printf(" -h             : Print this help.\n");
    printf(" -r             : Run in RECEIVER mode.\n");
    printf(" -s             : Run in SENDER mode.\n");

    return 0;
}

int
parse_args (int argc, char *argv[])
{
    int     c = 0;
    while ((c = getopt (argc, argv, "hrsvdi:f:")) != -1) {
        switch(c) {
            case 'r':
                glopts.runmode = RUNMODE_RECEIVER;
                break;
            case 's':
                glopts.runmode = RUNMODE_SENDER;
                break;
            case 'd':
                glopts.loglevel = LOG_LEVEL_DEBUG;
                break;
            case 'v':
                glopts.loglevel = LOG_LEVEL_VERBOSE;
                break;
            case 'i':
                glopts.ifname = optarg;
                break;
            case 'f':
                glopts.pcapfile = optarg;
                break;
            case 'h':
                print_help();
                exit(0);
            default:
                printf("Invalid Option: %c\n", c);
                print_usage();
                exit(1);
                break;
        }
    }

    if (glopts.ifname == NULL ||
        glopts.pcapfile == NULL) {
        print_usage();
        exit(1);
    }

    return 0;
}

int
init_iovec ()
{
    bzero(&glinfo.iovecs, sizeof(glinfo.iovecs));

    for (int i = 0; i < MAX_NUM_PKTS; i++) {
        glinfo.iovecs[i].iov_base = glinfo.pktinfo[i].pkt;
        glinfo.iovecs[i].iov_len = MAX_PKT_SIZE;
    }

    return 0;
}

int
init_mmsg ()
{
    bzero(&glinfo.mmsgs, sizeof(glinfo.mmsgs));
    
    for (int i = 0; i < MAX_NUM_PKTS; i++) {
        struct mmsghdr *msg = &glinfo.mmsgs[i];

        msg->msg_hdr.msg_iov = &(glinfo.iovecs[i]);
        msg->msg_hdr.msg_iovlen = 1;
        msg->msg_hdr.msg_name = &glinfo.address;
        msg->msg_hdr.msg_namelen = sizeof(glinfo.address);
        msg->msg_hdr.msg_control = NULL;
        msg->msg_hdr.msg_controllen = 0;
        msg->msg_hdr.msg_flags = 0;
    }

    return 0;
}
int
init_glinfo ()
{
    struct ifreq    req;

    bzero(&glinfo, sizeof(glinfo));
    
    glinfo.loglevel = LOG_LEVEL_INFO;
    if (glopts.loglevel != LOG_LEVEL_NONE) {
        glinfo.loglevel = glopts.loglevel;
    }

    // Open RAW socket to send on
    glinfo.fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
    if (glinfo.fd == -1) {
        LOG_ERROR("Unable to open Raw Socket.");
        exit(1);
    }

    // Get the ifindex of this interface.
    bzero(&req, sizeof(req));
    strncpy(req.ifr_name, glopts.ifname, IFNAMSIZ - 1);
    if (ioctl(glinfo.fd, SIOCGIFINDEX, &req) < 0) {
        perror("SIOCGIFINDEX");
        exit(1);
    }
    glinfo.ifindex = req.ifr_ifindex;
    LOG_INFO("Interface Index = %#x", glinfo.ifindex);
        
    // Get the MAC address of this interface.
    bzero(&req, sizeof(req));
    strncpy(req.ifr_name, glopts.ifname, IFNAMSIZ - 1);
    if (ioctl(glinfo.fd, SIOCGIFHWADDR, &req) < 0) {
        perror("SIOCGIFHWADDR");
        exit(1);
    }
    maccpy(glinfo.srcmac.ether_addr_octet,
           ((uint8_t *)&req.ifr_hwaddr.sa_data));
    LOG_INFO("Interface Source MAC = %s", mac2str(glinfo.srcmac));

    init_iovec();
    init_mmsg();

    return 0;
}


