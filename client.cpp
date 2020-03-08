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
        std::cout << "Usage `./Client some_number` in terminal" << std::endl;
        return 0;
    }

    try{
        io_service io_serv; 
        ip::tcp::socket socket(io_serv);
        ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), atoi(argv[1]));
        ConHandler client(io_serv, endpoint, socket);
        client.run();
    }
    catch(const std::exception& e){
        std::cout << "Closing program" << std::endl;
        return 0;
    }

    return 0; 
} 