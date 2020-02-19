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

using namespace boost::asio;


#ifndef CON_HANDLER_INCLUDE
#define CON_HANDLER_INCLUDE


class ConHandler
{
public:
  ConHandler(io_service& io, boost::shared_ptr<ip::tcp::socket>  socket)
    : strand_(io),
        timer1_(io, boost::posix_time::seconds(10)),
        timer2_(io, boost::posix_time::seconds(10))
    {
        socket_ptr = socket;
        std::cout << "You are connected!" << std::endl
            << "To exit chat just type exit" << std::endl;
    }

    ~ConHandler()
    {
        
        timer1_.cancel();
        timer2_.cancel();
        reading_thread.interrupt();
        writing_thread.interrupt();
        reading_thread.~thread();
        writing_thread.~thread();
        socket_ptr->close();
        std::cout << "Exiting chat" << std::endl;
    }

    // reading using socket
    void recive_data() { 
        streambuf buf; 
        try{
            read_until(*socket_ptr, buf, ""); 
        }
        catch(std::exception e){
            return;
        }
        
        data = buffer_cast<const char*>(buf.data()); 
        if(data == "exit"){
            std::cout << "User finised connection" << std::endl;
            delete this;
        }
        else{
            std::cout << ": " << data << std::endl;
    
            timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
            timer1_.async_wait(strand_.wrap(boost::bind(&ConHandler::recive_data, this)));
        }
    } 

    // writing using socket
    void send_message() { 
        
        write(*socket_ptr, 
            buffer(message)); 
        
        if(message == "exit"){
            std::cout << "User finised connection" << std::endl;
            delete this;
        }
        else{
            timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
            timer2_.async_wait(strand_.wrap(boost::bind(&ConHandler::send_message, this)));
        }
    } 

    // method for reading thread
    void reading() {
        while(boost::this_thread::interruption_requested() == false){
            recive_data();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }

    // method for writing thread
    void writing() {
        while(boost::this_thread::interruption_requested() == false){
            getline(std::cin, message);
            send_message();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }

    
    void run()
    {
        reading_thread = boost::thread(boost::bind(&ConHandler::reading, this));
        writing_thread = boost::thread(boost::bind(&ConHandler::writing, this));
        reading_thread.join();
        writing_thread.join();
    }

    std::string get_data(){ return data; }
    std::string get_message(){ return message; }
    void set_message(std::string new_message){ message = new_message; }
  


private:
    boost::asio::io_service::strand strand_;
    boost::shared_ptr<ip::tcp::socket> socket_ptr;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;
    std::string data, message;

    boost::thread reading_thread;
    boost::thread writing_thread;

};

#endif