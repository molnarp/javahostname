/*
Copyright 2016 Peter Molnar (mp6633@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

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
        if (res->ai_family == AF_INET) {
            struct sockaddr_in *addr4;
            addr4 = (struct sockaddr_in *) res->ai_addr;
            inet_ntop(res->ai_family, &addr4->sin_addr, addr_str, res->ai_addrlen);
        }
        else if (res->ai_family == AF_INET6) {
            struct sockaddr_in6 *addr6;
            addr6 = (struct sockaddr_in6 *) res->ai_addr;
            inet_ntop(res->ai_family, &addr6->sin6_addr, addr_str, res->ai_addrlen);
        }
        printf("ip address: %s\n", addr_str);

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

        printf("resolved hostname: %s\n", hostname);
    }
}
