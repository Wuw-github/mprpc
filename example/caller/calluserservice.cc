#include <iostream>

#include "rpcheader.pb.h"
#include "user.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    // rpc request
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