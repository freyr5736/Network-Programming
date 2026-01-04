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
            int sock; // our socket 
            int connection;

        public:
            // default constructor
            my_socket(int domain, int service, int protocol, int port, u_long interface);

            // for different classes to implement.
            virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;

            // testing functions
            void test_connection(int);

            // getter functions
            struct sockaddr_in get_address();
            int get_socket();
            int get_connection();

            // setter functions
            int set_connection (int con);
    };
}


#endif
