#include <iostream>

#include "rpcheader.pb.h"
#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    // rpc request to the register service
    fixbug::RegisterRequest register_request;
    register_request.set_id(10001);
    register_request.set_name("zhangsan");
    register_request.set_pwd("123456");
    fixbug::RegisterResponse register_response;
    stub.Register(nullptr, &register_request, &register_response, nullptr);

    if (register_response.result().errcode() == 0)
    {
        std::cout << "rpc register success" << std::endl;
    }
    else
    {
        std::cout << "rpc register failed, errcode: " << register_response.result().errmsg() << std::endl;
    }

    // rpc request to the login service
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");

    // rpc response
    fixbug::LoginResponse response;

    // sync rpc call
    stub.Login(nullptr, &request, &response, nullptr);

    // check response
    if (response.result().errcode() == 0)
    {
        std::cout << "rpc login success" << std::endl;
    }
    else
    {
        std::cout << "rpc login failed, errcode: " << response.result().errmsg() << std::endl;
    }
    return 0;
}