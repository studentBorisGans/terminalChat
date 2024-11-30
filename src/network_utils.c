#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include "../include/network_utils.h"

// Function to get and display all local IP addresses
void get_local_ip()
{
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];

    // Get a linked list of network interfaces
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    printf("Local IP addresses:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        // Check if the address is IPv4
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                            host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == 0)
            {
                printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, host);
            }
        }
    }

    freeifaddrs(ifaddr); // Free the memory allocated by getifaddrs
}

// Function to resolve a hostname or domain name to an IP address
int resolve_hostname(const char *hostname, char *ip_buffer, size_t buffer_size)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    // Resolve the hostname to an address
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
    {
        perror("getaddrinfo");
        return -1;
    }

    // Convert the address to a human-readable string
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    inet_ntop(res->ai_family, &(ipv4->sin_addr), ip_buffer, buffer_size);

    freeaddrinfo(res); // Free the linked list allocated by getaddrinfo
    return 0;          // Success
}
