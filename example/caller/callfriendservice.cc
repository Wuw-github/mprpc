#include <iostream>

#include "rpcheader.pb.h"
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);

    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());

    // rpc request to the register service
    fixbug::GetFriendListRequest friend_request;
    friend_request.set_userid(10001);
    fixbug::GetFriendListResponse friend_response;

    MprpcController controller;

    stub.GetFriendList(&controller, &friend_request, &friend_response, nullptr);

    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (friend_response.result().errcode() == 0)
        {
            std::cout << "rpc register success" << std::endl;

            for (int i = 0; i < friend_response.friendid_size(); ++i)
            {
                std::cout << "friend: " << friend_response.friendid(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc register failed, errcode: " << friend_response.result().errmsg() << std::endl;
        }
    }
}