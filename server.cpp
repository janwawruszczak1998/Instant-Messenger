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
  Server(io_service& io, boost::shared_ptr<ip::tcp::socket>  socket)
    : strand_(io),
        timer1_(io, boost::posix_time::seconds(10)),
        timer2_(io, boost::posix_time::seconds(10))
    {
        socket_ptr = socket;
        std::cout << "You are connected!" << std::endl;
    }

    ~Server()
    {
        reading_thread.interrupt();
        writing_thread.interrupt();
        reading_thread.~thread();
        writing_thread.~thread();

        socket_ptr->close();
        std::cout << "Connection lost" << std::endl;
    }

    // reading using socket
    void getData() { 
        streambuf buf; 
        try{
            read_until(*socket_ptr, buf, ""); 
        }
        catch(std::exception e){
            return;
        }
        
        std::string data = buffer_cast<const char*>(buf.data()); 
        if(data == "exit"){
            std::cout << "User finised connection" << std::endl;
            this->~Server();
            return;
        }
        std::cout << ": " << data << std::endl;

    
        timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
        timer1_.async_wait(strand_.wrap(boost::bind(&Server::getData, this)));
    } 

    // writing using socket
    void sendData() { 
        std::string message;
        getline(std::cin, message);
        write(*socket_ptr, 
            buffer(message)); 
        
        if(message == "exit"){
            std::cout << "User finised connection" << std::endl;
            this->~Server();
            return;
        }
    
        timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
        timer2_.async_wait(strand_.wrap(boost::bind(&Server::sendData, this)));
    } 

    // method for reading thread
    void reading() {
        while(true){
            getData();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }

    // method for writing thread
    void writing() {
        while(true){
            sendData();
            boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
        }
    }

    
    void run()
    {
        reading_thread = boost::thread(boost::bind(&Server::reading, this));
        writing_thread = boost::thread(boost::bind(&Server::writing, this));
        reading_thread.join();
        writing_thread.join();
    }
  


private:
    boost::asio::io_service::strand strand_;
    boost::shared_ptr<ip::tcp::socket> socket_ptr;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;

    boost::thread reading_thread;
    boost::thread writing_thread;

};
  

  

int main(int argc, char* argv[])
{ 
    
     io_service io_service; 
    
    // Listening for any new incomming connection at port with IPv4 protocol 
    ip::tcp::endpoint endpoint(ip::tcp::v4(), atoi(argv[1]));

    ip::tcp::acceptor acceptor_server( 
        io_service, endpoint); 

    // smart pointer, its impossible to copy socket (it will be mess), so need to pass pointer
    boost::shared_ptr<ip::tcp::socket> socket_ptr(new ip::tcp::socket(io_service));
    
    // waiting for connection 
    try{
        acceptor_server.accept(*socket_ptr); 
    }
    catch(std::exception e){
        std::cout << e.what();
    }

    // running chat
    Server server(io_service, socket_ptr);

    io_service.run();
    server.run();

    return 0; 
} 