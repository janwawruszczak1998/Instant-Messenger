#include<ctime>
#include<iostream>
#include<string>
#include<list>
#include<queue>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>
#include"Message.hpp"
#include"ChatRoom.hpp"
#include"ChatParticipant.hpp"


using namespace boost::asio;   
// Driver program for receiving data from buffer 
std::string getData(ip::tcp::socket& socket) 
{ 
    streambuf buf; 
    read_until(socket, buf, "\n"); 
    std::string data = buffer_cast<const char*>(buf.data()); 
    return data; 
} 
  
// Driver program to send data 
void sendData(ip::tcp::socket& socket, const std::string& message) 
{ 
    write(socket, 
          buffer(message + "\n")); 
} 
  
int main(int argc, char* argv[]) 
{ 
    io_service io_service; 
  
    // Listening for any new incomming connection 
    // at port 8080 with IPv4 protocol 
    ip::tcp::endpoint endpoint(ip::tcp::v4(), atoi(argv[1]));
    ip::tcp::acceptor acceptor_server( 
        io_service, endpoint); 
  
    // Creating socket object 
    ip::tcp::socket server_socket(io_service); 
  
    // waiting for connection 
    acceptor_server.accept(server_socket); 
  
    // Reading username 
    std::string u_name = getData(server_socket); 
    // Removing "\n" from the username 
    u_name.pop_back(); 
  
    // Replying with default mesage to initiate chat 
    std::string response, reply; 
    reply = "Hello there!";
    sendData(server_socket, reply); 
  
    while (true) { 
  
        // Fetching response 
        response = getData(server_socket); 
  
        // Popping last character "\n" 
        response.pop_back(); 
  
        // Validating if the connection has to be closed 
        if (response == "exit") { 
            std::cout << "User left!" << std::endl; 
            break; 
        } 
        std::cout << ": " << response << std::endl; 
  
        // Reading new message from input stream 
        
        getline(std::cin, reply); 
        sendData(server_socket, reply); 
  
        if (reply == "exit") 
            break; 
    } 
    return 0; 
} 