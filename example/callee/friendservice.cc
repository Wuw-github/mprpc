#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class FriendService : public fixbug::FriendServiceRpc
{

public:
    std::vector<std::string> getFriendListImpl(uint32_t id)
    {
        std::cout << "Doing local service: GetFriendLists" << std::endl;
        std::cout << "id: " << id << std::endl;
        std::vector<std::string> friends = {"zhangsan", "lisi", "wangwu"};
        return friends;
    }

    void GetFriendList(google::protobuf::RpcController *controller,
                       const ::fixbug::GetFriendListsRequest *request,
                       ::fixbug::GetFriendListsResponse *response,
                       ::google::protobuf::Closure *done) override
    {
        // get request parameters
        uint32_t id = request->id();

        // local service
        std::vector<std::string> friends = getFriendListImpl(id);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        // write response
        for (const std::string &friend_name : friends)
        {
            std::string *p = response->add_friend();
            *p = friend_name;
        }

        // callback, serialize response and network send
        done->Run();
    }
}

int
main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    RpcProvider provider;
    provider.NotifyService(new FriendService());
    provider.Run();

    return 0;
}