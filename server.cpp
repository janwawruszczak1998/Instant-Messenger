#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>



using namespace boost::asio;


class Server
{
public:
  Server(boost::asio::io_service& io, boost::shared_ptr<ip::tcp::socket>  socket)
    : strand_(io),
        timer1_(io, boost::posix_time::seconds(10)),
        timer2_(io, boost::posix_time::seconds(10))
    {
    socket_ptr = socket;
    std::cout << "You are connected!" << std::endl;
    }

  ~Server()
  {
    std::cout << "Connection lost" << std::endl;
  }

  void getData() { 
   // std::cout << "read" << std::endl;
    streambuf buf; 
    read_until(*socket_ptr, buf, ""); 
    std::string data = buffer_cast<const char*>(buf.data()); 
    std::cout << ": " << data << std::endl;
    
    timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
    timer1_.async_wait(strand_.wrap(boost::bind(&Server::getData, this)));
} 

void sendData() { 
    std::string message;
    getline(std::cin, message);
    write(*socket_ptr, 
          buffer(message)); 
    
    timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
    timer2_.async_wait(strand_.wrap(boost::bind(&Server::sendData, this)));
    
} 

void TimeoutThread() {
        while(true){
            getData();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        }
    }

    void WorkerThread() {
        while(true){
            sendData();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        }
    }

    
    void run()
    {
        timeoutThread = boost::thread(boost::bind(&Server::TimeoutThread, this));
        workerThread  = boost::thread(boost::bind(&Server::WorkerThread, this));
        workerThread.join();
        timeoutThread.join();
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
    
    // Listening for any new incomming connection 
    // at port 8080 with IPv4 protocol 
    
    ip::tcp::endpoint endpoint(ip::tcp::v4(), atoi(argv[1]));

    ip::tcp::acceptor acceptor_server( 
        io_service, endpoint); 

    boost::shared_ptr<ip::tcp::socket> socket_ptr(new ip::tcp::socket(io_service));
    // waiting for connection 
    
    try{
        acceptor_server.accept(*socket_ptr); 
    }
    catch(std::exception e){
        std::cout << e.what();
    }

    

    Server server(io_service, socket_ptr);

    io_service.run();
    server.run();

    return 0; 
} 