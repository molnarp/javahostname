//
// Created by mp on 9/12/16.
//
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <memory.h>
#include <arpa/inet.h>

int main() {
    char hostname[64];

    gethostname(hostname, 64);
    printf("gethostname: %s\n", hostname);

    struct addrinfo  hints, *res;
    int error;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC;

    error = getaddrinfo(hostname, NULL, &hints, &res);

    if (error == 0) {
        char addr_str[res->ai_addrlen];
        inet_ntop(res->ai_family, res->ai_addr, addr_str, res->ai_addrlen);
        printf("IP address: %s\n", addr_str);

        /* host is known to name service */
        error = getnameinfo(res->ai_addr,
                            res->ai_addrlen,
                            hostname,
                            NI_MAXHOST,
                            NULL,
                            0,
                            NI_NAMEREQD);

        /* if getnameinfo fails hostname is still the value
           from gethostname */

        freeaddrinfo(res);

        printf("Final hostname: %s\n", hostname);
    }
}
