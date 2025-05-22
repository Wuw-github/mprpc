#include <iostream>
#include <string>
#include <vector>
#include "user.pb.h"
#include "mprpcapplication.h"

// user service is a local service that provides two methods:
// 1. login
// 2. getFriendLists
class UserService : public fixbug::UserServiceRpc { // rpc provider

  public:
    bool loginImpl(std::string username, std::string password) {
        // login logic
        std::cout << "Doing local service: Login" << std::endl;
        std::cout << "username: " << username << std::endl;
        std::cout << "password: " << password << std::endl;
        return true;
    }

    /*
    Rewrite virtual function in the base rpc class
    1. caller -> Login(LoginRequest) -> muduo -> callee
    2. callee -> this->Login(LoginRequest)
    */
    void Login(google::protobuf::RpcController* controller,
               const ::fixbug::LoginRequest* request,
               ::fixbug::LoginResponse* response,
               ::google::protobuf::Closure* done) override 
    {
        // get request parameters
        std::string name = request->name();
        std::string pwd = request->pwd();
        
        // prescreen input ...

        // local service
        bool login_res = loginImpl(name, pwd);

        // write response
        fixbug::ResultCode* result = response->mutable_result();
        result->set_errcode(0);
        result->set_errmsg("");
        response->set_result(login_res);
        
        // callback, serialize response and network send 
        done->Run();
    }
};

int main(int argc, char* argv[]) {
    // initialization
    MprpcApplication::Init(argc, argv);

    RpcProvider provider; 
    provider.NotifyService(new UserService());

    provider.Run();

}