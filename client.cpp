#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>

#include <boost/asio.hpp>

#include "con_handler.hpp"

using namespace boost::asio;

  
int main(int argc, char* argv[]) 
{ 

    if(argc != 2){
        std::cout << "Usage `./Server some_number`" << std::endl;
        return 0;
    }
    
    try{
    io_service io_service; 
    ConHandler client(io_service);
    
        // connecting 
        client.get_socket()
            .connect( 
                ip::tcp::endpoint( 
                    ip::address::from_string("127.0.0.1"), 
                    atoi(argv[1]))); 
        // running chat
        client.run();
    }
    catch(std::exception e){
        std::cout << "Closing program" << std::endl;
        return 0;
    }

    return 0; 
} 