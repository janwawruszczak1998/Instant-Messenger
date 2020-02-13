#include<ctime>
#include<iostream>
#include<string>
#include<boost/asio.hpp>
#include<boost/bind.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>

using namespace boost::asio;


//message about daytime
std::string make_daytime_string(){
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

class ConnectionHandler : public boost::enable_shared_from_this<ConnectionHandler>{
private:
    ip::tcp::socket socket_;
    std::string message_ = "Hello there!\n";
    char data[256];

    ConnectionHandler(io_service& io_service)
    :socket_(io_service)
    {
    }

public:
    typedef boost::shared_ptr<ConnectionHandler> pointer;

    //creating new pointer of this class - factory
    //need to be static - creating this in server constructor
    static pointer create_pointer(io_service& io_service){
        return pointer(new ConnectionHandler(io_service));
    }

    ip::tcp::socket& get_socket(){
        return socket_;
    }
    
    void start(){
        
        //reading
        socket_.async_read_some(buffer(data, 256),
            boost::bind(&ConnectionHandler::handle_read, 
                shared_from_this(),
                placeholders::error,
                placeholders::bytes_transferred
        ));
        
        //writing
        socket_.async_write_some(buffer(message_, 256),
            boost::bind(&ConnectionHandler::handle_write, 
                shared_from_this(),
                placeholders::error,
                placeholders::bytes_transferred
        ));
        
    }

    void handle_read(const boost::system::error_code& error,
                    size_t bytes_transferred){
        if(!error){
            std::cout << data << std::endl;
        }
        else{
            std::cerr << "error" << error.message() << std::endl;
            socket_.close();
        }
    }

    void handle_write(const boost::system::error_code& error,
                    size_t bytes_transferred){
        if(error){
            std::cerr << "error" << error.message() << std::endl;
            socket_.close();
        }
        else{
            std::cout << "Helo from server" << std::endl;
        }
    }
};


class Server{
private:
    ip::tcp::acceptor acceptor_;

    void accept(){
        //create a socket and initiates async accept operations
        ConnectionHandler::pointer connection 
            = ConnectionHandler::create_pointer(acceptor_.get_io_service());
        
        
        //accept operation and wait
        acceptor_.async_accept(connection->get_socket(),
            boost::bind(&Server::handle_accept, this, connection,
            placeholders::error));
            
    }

public:
    Server(io_service& io_service)
    : acceptor_(io_service, ip::tcp::endpoint(ip::tcp::v4(), 8080))
    {
        accept();
    }

    void handle_accept(ConnectionHandler::pointer connection, 
        const boost::system::error_code& error
        ){

        if(!error){
            connection->start();
        }
        accept();

    }
};

int main(){
    try{
        io_service io_service;
        Server server(io_service);
        io_service.run();
    }
    catch(std::exception exception){
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}