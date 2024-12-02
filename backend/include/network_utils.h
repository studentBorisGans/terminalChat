#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stddef.h> // For size_t

/**
 * @brief Displays all local IP addresses of the current machine.
 *
 * Uses the `getifaddrs` function to iterate through all network interfaces
 * and prints the IPv4 addresses associated with them.
 */
void get_local_ip();

/**
 * @brief Resolves a hostname or domain name to an IP address.
 *
 * Converts a hostname (e.g., "example.com") or a string representation of
 * an IP address (e.g., "192.168.1.1") into its corresponding IPv4 address.
 *
 * @param hostname The hostname or IP string to resolve.
 * @param ip_buffer A buffer to store the resolved IP address.
 * @param buffer_size The size of the ip_buffer.
 * @return int Returns 0 on success, -1 on failure.
 */
int resolve_hostname(const char *hostname, char *ip_buffer, size_t buffer_size);

#endif // NETWORK_UTILS_H
