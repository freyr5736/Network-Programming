#include "binding_socket.hpp"

// default constructor
custom_socket::binding_socket::binding_socket(int domain, int service, int protocol, int port, u_long interface) : my_socket(domain,service,protocol,port,interface){
    set_connection (connect_to_network(get_socket(),get_address()));
    test_connection(get_connection());
}

// definition for connect_to_network on server side
int custom_socket::binding_socket::connect_to_network(int sock, struct sockaddr_in address){
    return bind(sock,(struct sockaddr* )&address, sizeof(address));
}