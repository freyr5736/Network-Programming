#ifndef  connecting_socket_hpp
#define  connecting_socket_hpp

#include <stdio.h>

#include "my_socket.hpp"

namespace custom_socket {
    class connecting_socket: public my_socket{
        public:

            connecting_socket(int domain, int service, int protocol, int port, u_long interface);
            
            int connect_to_network(int sock, struct sockaddr_in address); // from my_socket

    };
}

#endif 