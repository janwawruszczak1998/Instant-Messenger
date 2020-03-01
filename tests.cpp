#include <gtest/gtest.h>
#include "con_handler.hpp"

std::unique_ptr<ip::tcp::acceptor> get_acceptor_ptr(
    io_service& io_serv,
    ip::tcp::endpoint endpoint_server){
    auto acc_ptr = new ip::tcp::acceptor(io_serv, endpoint_server);
    return std::unique_ptr<ip::tcp::acceptor>(acc_ptr);
}

class ConHandlerTest : public ::testing::Test
{
protected:
    io_service io_serv; 

    // fixture allows only object created by default constructor,
    // and since no default constructor exists and
    // acceptor is nessesary to connect server and client in test,
    // need to use a pointer
    std::unique_ptr<ip::tcp::acceptor> acceptor_ptr;
    
    virtual void SetUp(){
        ip::tcp::endpoint endpoint_server(ip::tcp::v4(), 9999);
        acceptor_ptr = get_acceptor_ptr(io_serv, endpoint_server);            
    }

    virtual void TearDown(){
    }

};

TEST_F(ConHandlerTest, is_seding_data_correct_test){

    // server and client need to be created here,
    // fixture allows only default contructors,
    // adding implicit default constructor in con_handler.hpp is unreasonable
    // when no default constructor exists for class strand and socket
    ConHandler server(io_serv);
    ConHandler client(io_serv);
  
    client.get_socket()
        .connect( 
            ip::tcp::endpoint( 
                ip::address::from_string("127.0.0.1"), 9999));
  
    // Connection
    acceptor_ptr->accept(server.get_socket()); 


    // sending empty line test
    server.set_message("\n");
    server.send_message();
    client.recive_data();

    ASSERT_EQ("\n", client.get_data());


    // sending single word test
    server.set_message("stream_test");
    server.send_message();
    client.recive_data();

    ASSERT_EQ("stream_test", client.get_data());

    // sending some text test
    server.set_message("some long text");
    server.send_message();
    client.recive_data();

    ASSERT_EQ("some long text", client.get_data());
}


int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}