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
        std::cout << "Usage `./Server some_number` in terminal" << std::endl;
        return 0;
    }
    
    try{
        io_service io_serv; 
        ip::tcp::socket socket(io_serv);
        ip::tcp::endpoint endpoint(ip::tcp::v4(), atoi(argv[1]));
        ip::tcp::acceptor acceptor_server(io_serv, endpoint);
        // listening for any new incomming connection at port with IPv4 protocol 
        ConHandler server(io_serv, endpoint, socket, acceptor_server); 
        server.run();
    }
    catch(const std::exception& e){
        std::cout << "Closing program" << std::endl;
        return 0;
    }

    return 0; 

} 