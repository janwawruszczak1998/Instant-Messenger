#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>

#include "con_handler.hpp"


using namespace boost::asio;

int main(int argc, char* argv[])
{
    io_service io_serv; 
    
    // listening for any new incomming connection at port with IPv4 protocol 
    ip::tcp::endpoint endpoint(ip::tcp::v4(), atoi(argv[1]));

    ip::tcp::acceptor acceptor_server( 
        io_serv, endpoint); 

    // smart pointer, its impossible to copy socket (it will be mess), so need to pass pointer
    boost::shared_ptr<ip::tcp::socket> socket_ptr(new ip::tcp::socket(io_serv));

    // waiting for connection 
    try{
        acceptor_server.accept(*socket_ptr); 
    }
    catch(std::exception e){
        std::cout << e.what();
    }

    // running chat
    ConHandler* server = new ConHandler(io_serv, socket_ptr);
    server->run();
    
    return 0; 
} 