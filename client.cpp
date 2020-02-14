#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace boost::asio;

  
std::string getData(ip::tcp::socket& socket) 
{ 
    streambuf buf; 
    read_until(socket, buf, "\n"); 
    std::string data = buffer_cast<const char*>(buf.data()); 
    return data; 
} 
  
void sendData(ip::tcp::socket& socket, const std::string& message) 
{ 
    write(socket, 
          buffer(message + "\n")); 
} 
  
int main(int argc, char* argv[]) 
{ 
    io_service io_service; 
    // socket creation 
    ip::tcp::socket client_socket(io_service); 
  
    client_socket 
        .connect( 
            ip::tcp::endpoint( 
                ip::address::from_string("127.0.0.1"), 
                atoi(argv[1]))); 
  
    // Getting username from user 
    std::string reply = "Hi!", response; 
  
    // Sending username to another end 
    // to initiate the conversation 
    sendData(client_socket, reply); 
  
    // Infinite loop for chit-chat 
    while (true) { 
  
        // Fetching response 
        response = getData(client_socket); 
  
        // Popping last character "\n" 
        response.pop_back(); 
  
        // Validating if the connection has to be closed 
        if (response == "exit") { 
            std::cout << "Connection terminated" << std::endl; 
            break; 
        } 
        std::cout << ": " << response << std::endl; 
  
        // Reading new message from input stream 
        getline(std::cin, reply); 
        sendData(client_socket, reply); 
  
        if (reply == "exit") 
            break; 
    } 
    return 0; 
} 