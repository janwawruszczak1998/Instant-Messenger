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
    
    ConHandler(io_service& io, ip::tcp::endpoint& endpoint, 
    ip::tcp::socket& socket,  ip::tcp::acceptor& acceptor_server)
    : strand_(io),
        socket_(socket),
        running_flag(true) 
    {
        acceptor_server.accept(socket);
        std::cout << "You are connected!" << std::endl
            << "To exit chat just type exit" << std::endl;

    }
    ConHandler(io_service& io, ip::tcp::endpoint& endpoint, ip::tcp::socket& socket)
    : strand_(io),
        socket_(socket),
        running_flag(true) 
    {
        socket_.connect(endpoint); 
        std::cout << "You are connected!" << std::endl
            << "To exit chat just type exit" << std::endl;

    }
    ~ConHandler()
    {
        strand_.get_io_service().stop();
        socket_.close();
    }

    // reading using socket
    void recive_data() { 
        streambuf buf; 
        try{
            read_until(socket_, buf, "");
        }
        catch(const std::exception& e){
            std::cout << e.what() << std::endl;
            return;
        }
        
        data = buffer_cast<const char*>(buf.data()); 
        if(data == "exit"){
            running_flag = false;
            std::cout << "User finished connection" << std::endl;
            write(socket_, 
                buffer("exit")); 
        }
        else{
            std::cout << ": " << data << std::endl;
        }
    } 

    // writing using socket
    void send_message() { 
        try{
            if(message == "exit"){
                write(socket_, buffer(message)); 
                running_flag = false;
            }
            else write(socket_, buffer(message));  
        }
        catch(const std::exception& e){
            std::cout << e.what() << std::endl;
            return;
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


private:
    boost::asio::io_service::strand strand_;
    ip::tcp::socket& socket_;
    std::string data, message;

    std::thread reading_thread;
    std::thread writing_thread;
    std::atomic_bool running_flag;

};

#endif