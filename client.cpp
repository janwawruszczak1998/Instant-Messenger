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

    io_service io_service; 
    boost::shared_ptr<ip::tcp::socket> socket_ptr(new ip::tcp::socket(io_service));
    
    // connecting 
    socket_ptr->
        connect( 
            ip::tcp::endpoint( 
                ip::address::from_string("127.0.0.1"), 
                atoi(argv[1]))); 
  
    // running chat
    ConHandler* client = new ConHandler(io_service, socket_ptr);
    client->run();

    return 0; 
} 