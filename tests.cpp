#include <gtest/gtest.h>
#include "con_handler.hpp"

namespace my {
namespace project {
namespace {

class ConHandlerTest : public ::testing::Test
{
protected:

    ConHandlerTest()
    : socket_server(io_serv),
        endpoint_server(ip::tcp::v4(), 9999),
        acceptor_server(io_serv, endpoint_server),
        socket_client(io_serv),
        endpoint_client(ip::address::from_string("127.0.0.1"), 9999)
        
    {
    }
    ~ConHandlerTest(){}

    void SetUp() override {
    }

    void TearDown() override {
    }

    io_service io_serv; 
    // Server
    ip::tcp::socket socket_server;
    ip::tcp::endpoint endpoint_server;
    ip::tcp::acceptor acceptor_server;
    
    // Client
    ip::tcp::socket socket_client;
    ip::tcp::endpoint endpoint_client;

};


TEST_F(ConHandlerTest, sending_empty_line_test){

    ConHandler server(io_serv, endpoint_server, socket_server, acceptor_server);
    ConHandler client(io_serv, endpoint_client, socket_client);

    // sending empty line test
    server.set_message("\n");
    server.send_message();
    client.recive_data();

    ASSERT_EQ("\n", client.get_data());
}

TEST_F(ConHandlerTest, sending_single_word_test){

    ConHandler client(io_serv, endpoint_client, socket_client);
    ConHandler server(io_serv, endpoint_server, socket_server, acceptor_server);

    // sending single word test
    server.set_message("stream_test");
    server.send_message();
    client.recive_data();

    ASSERT_EQ("stream_test", client.get_data());
}

TEST_F(ConHandlerTest, sending_long_text_test){
    
    ConHandler client(io_serv, endpoint_client, socket_client);
    ConHandler server(io_serv, endpoint_server, socket_server, acceptor_server);

    // sending some text test
    server.set_message("some long text");
    server.send_message();
    client.recive_data();

    ASSERT_EQ("some long text", client.get_data());
}


}   // namespace 
}   // namespace project
}   // namespace my

int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}
