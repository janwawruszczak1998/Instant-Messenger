#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>

#include <boost/asio.hpp>

#include "con_handler.hpp"

using namespace boost::asio;

int main(int argc, char* argv[])
{
    // program requires 2 arguments
    if(argc != 2){
        std::cout << "Usage `./Server some_number`" << std::endl;
        return 0;
    }
    try{
        io_service io_serv; 
    
        // listening for any new incomming connection at port with IPv4 protocol 
        ip::tcp::endpoint endpoint(ip::tcp::v4(), atoi(argv[1]));

        ip::tcp::acceptor acceptor_server( 
            io_serv, endpoint); 

        ConHandler server(io_serv);

        // running chat
        acceptor_server.accept(server.get_socket()); 
        server.run();

    }
    catch(std::exception e){
        std::cout << "Closing program" << std::endl;
        return 0;
    }

    return 0; 

} 