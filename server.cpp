#include<ctime>
#include<iostream>
#include<string>
#include<boost/asio.hpp>

using namespace boost::asio;

//message about daytime
std::string make_daytime_string(){
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

int main(){
    try{
        io_service io_service;

        //acceptator needed to listen for new connections
        ip::tcp::acceptor acceptator(io_service, ip::tcp::endpoint(ip::tcp::v4(), 8080)); //port 8080
        while(true){
            ip::tcp::socket socket(io_service);
            acceptator.accept(socket);
            //some message - current time
            std::string message = make_daytime_string();

            //when client accessing server
            boost::system::error_code error;
            write(socket, buffer(message), error);

        }
    }
    catch(std::exception exception){
        std::cerr << exception.what() << std::endl;
    }
}