#ifndef my_socket_hpp
#define my_socket_hpp

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


namespace custom_socket {
    class my_socket{
        private:
            struct sockaddr_in address;
            int sock;
            int connection;

        public:
            my_socket(int domain, int service, int protocol, int port, u_long interface);

            virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;

            void test_connection(int);

            struct sockaddr_in get_address();
            int get_socket();
            int get_connection();
    };
}






#endif
