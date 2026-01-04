#include "my_socket.hpp"

// default constructor
custom_socket::my_socket::my_socket(int domain, int service, int protocol, int port, u_long interface){
    
    // address structure
    address.sin_family = domain;
    address.sin_port = htons(port);;
    address.sin_addr.s_addr = htonl(interface);

    // socket
    sock = socket(domain,service,protocol);
    test_connection(sock);

    // network connection
    connection = connect_to_network(sock,address);
    test_connection(connection);

}

// virtual fucntion to test connection
void custom_socket::my_socket::test_connection(int item_to_test){
    if(item_to_test<0){
        perror("Failed to connect . . .");
        exit(EXIT_FAILURE);
    }
}

// getter functions
sockaddr_in custom_socket::my_socket::get_address(){
    return address;
}

int custom_socket::my_socket::get_connection(){
    return connection;
}

int custom_socket::my_socket::get_socket(){
    return sock;
}


