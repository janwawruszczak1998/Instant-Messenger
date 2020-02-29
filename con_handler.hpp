#ifndef CON_HANDLER_INCLUDE
#define CON_HANDLER_INCLUDE

#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <thread>

#include <boost/asio.hpp>


using namespace boost::asio;


class ConHandler
{
public:
  ConHandler(io_service& io)
    : strand_(io),
        socket(io),
        running_flag(true) 
    {
        std::cout << "You are connected!" << std::endl
            << "To exit chat just type exit" << std::endl;
    }

    ~ConHandler()
    {
        strand_.get_io_service().stop();
        socket.close();
    }

    // reading using socket
    void recive_data() { 
        streambuf buf; 
        try{
            read_until(socket, buf, ""); 
        }
        catch(std::exception e){
            return;
        }
        
        data = buffer_cast<const char*>(buf.data()); 
        if(data == "exit"){
            running_flag = false;
            std::cout << "User finised connection" << std::endl;
            reading_thread.detach();
            writing_thread.detach();
            reading_thread.~thread();
            writing_thread.~thread();
        }
        else{
            std::cout << ": " << data << std::endl;
        }
    } 

    // writing using socket
    void send_message() { 
        try{
            write(socket, 
                buffer(message)); 
        }
        catch(std::exception e){
            return;
        }

        
        
        if(message == "exit"){
            running_flag = false;
            std::cout << "User finished connection" << std::endl;
            reading_thread.detach();
            writing_thread.detach();
            reading_thread.~thread();
            writing_thread.~thread();
            //this->~ConHandler();
        }
        
    } 

    // method for reading thread
    void reading() {
        while(running_flag){
            recive_data();
        }
    }

    // method for writing thread
    void writing() {
        while(running_flag){
            getline(std::cin, message);
            send_message();
        }
    }

    
    void run()
    {
        reading_thread = std::thread{[this](){ this->reading(); } };
        writing_thread = std::thread{[this](){ this->writing(); } };
        reading_thread.join();
        writing_thread.join();
    }


    std::string get_data(){ return data; }
    std::string get_message(){ return message; }
    void set_message(std::string new_message){ message = new_message; }
    ip::tcp::socket& get_socket() { return socket; }
  


private:
    boost::asio::io_service::strand strand_;
    ip::tcp::socket socket;
    std::string data, message;

    std::thread reading_thread;
    std::thread writing_thread;
    std::atomic_bool running_flag;

};

#endif