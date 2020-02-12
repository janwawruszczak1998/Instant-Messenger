#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace boost::asio;

int main(int argc, char* argv[])
{
    try{
        if(argc != 2){
            std::cerr << "./ClientMessenger <number of port>" << std::endl;
            return 1;
        }
    std::cout << "running\n";
        //using asio need to have at least one io_service
        boost::asio::io_service io_service;
        //need to turn server name into tcp endpoint
        ip::tcp::resolver resolver(io_service);
        //resolver takes a query ant turns it into a list of endpoints
        ip::tcp::resolver::query query(argv[1], "daytime");
        //list of endpoints is returned using iterator
        ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    std::cout << "running2\n";
        //create and connect socket
        ip::tcp::socket socket(io_service);
        socket.connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), atoi(argv[1])));
    std::cout << "running3\n";
        //boost::array can be used to store recived data
        while(true){
            boost::array<char, 128> buff;
            boost::system::error_code err;

            //len of array and eventually error
            size_t len = socket.read_some(buffer(buff), err);

            if(err == error::eof){
                continue;
            }
            else if(err){
                throw boost::system::system_error(err); //someting happended
            }
            std::cout.write(buff.data(), len);
        }
        std::cout << "running4\n";
    }
    catch(std::exception exception){
            std::cerr << exception.what() << std::endl;
    }

    return 0;
}
