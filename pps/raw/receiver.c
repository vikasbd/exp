#include "common.h"

int
recv_packets ()
{
    int count = 0;
    while (1) {
        count = sendmmsg(glinfo.fd, glinfo.mmsgs, glinfo.num_pkts, 0);
        if (count < 0) {
            perror("recvmmsg");
            exit(1);
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

    recv_packets();

    return 0;
}
