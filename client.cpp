#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>


using namespace boost::asio;


class Client
{
public:
    Client(boost::asio::io_service& io, boost::shared_ptr<ip::tcp::socket>  socket)
    :   strand_(io),
        timer1_(io, boost::posix_time::seconds(1)),
        timer2_(io, boost::posix_time::seconds(1))
    {
        socket_ptr = socket;
        std::cout << "You are connected!" << std::endl;
    }
     ~Client()
    {
        std::cout << "Connection lost" << std::endl;
    }

void TimeoutThread() {
        while(true){
           // std::cout << "reader" << std::endl;
            getData();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(00));
        }
    }

    void WorkerThread() {
        while(true){
           // std::cout << "writer" << std::endl;
            sendData();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }

     void run()
    {
        timeoutThread = boost::thread(boost::bind(&Client::TimeoutThread, this));
        workerThread  = boost::thread(boost::bind(&Client::WorkerThread, this));
        workerThread.join();
        timeoutThread.join();
    }

    void getData() { 
        streambuf buf; 
        read_until(*socket_ptr, buf, ""); 
        std::string data = buffer_cast<const char*>(buf.data()); 
        std::cout << ": " << data << std::endl;

        timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
        timer1_.async_wait(strand_.wrap(boost::bind(&Client::getData, this)));
    } 

    void sendData() { 

        std::string message;
        getline(std::cin, message);
        write(*socket_ptr, 
            buffer(message)); 

        timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
        timer2_.async_wait(strand_.wrap(boost::bind(&Client::sendData, this)));
} 
  


  private:
    boost::asio::io_service::strand strand_;
    boost::shared_ptr<ip::tcp::socket> socket_ptr;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;

    boost::thread timeoutThread;
    boost::thread workerThread;

};

  
int main(int argc, char* argv[]) 
{ 

    io_service io_service; 
    boost::shared_ptr<ip::tcp::socket> socket_ptr(new ip::tcp::socket(io_service));
  
    socket_ptr->
        connect( 
            ip::tcp::endpoint( 
                ip::address::from_string("127.0.0.1"), 
                atoi(argv[1]))); 
  
    Client client(io_service, socket_ptr);
    io_service.run();
    client.run();

    return 0; 
} 