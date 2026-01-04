#include "connecting_socket.hpp"

// default constructor
custom_socket::connecting_socket::connecting_socket(int domain, int service, int protocol, int port, u_long interface) : my_socket(domain,service,protocol,port,interface){
    set_connection (connect_to_network(get_socket(),get_address()));
    test_connection(get_connection());
}

// definition for connect_to_network on client  side
int custom_socket::connecting_socket::connect_to_network(int sock, struct sockaddr_in address){
    return connect(sock,(struct sockaddr* )&address, sizeof(address));
}