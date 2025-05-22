#include "test.pb.h"
#include <iostream>
#include <string>

int main() {
    using namespace fixbug;

    // LoginResponse rsp;
    // rsp.set_success(1);
    // ResultCode* rc = rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("Unsuccess login.");

    GetFriendListsRequest request;
    request.set_userid(123456);

    GetFriendListsResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(0);
    
    User* user1 = rsp.add_friend_list();
    user1->set_name("zhangsan");
    user1->set_age(18);
    user1->set_sex(User::MAN);

    User* user2 = rsp.add_friend_list();
    user1->set_name("zhang si");
    user1->set_age(19);
    user1->set_sex(User::WOMAN);
    
    std::cout << rsp.friend_list_size() << std::endl;
}

int main1() {
    using namespace fixbug;

    LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    
    std::string serialized;
    if (request.SerializeToString(&serialized)) {
        std::cout << "Serialized LoginRequest: " << serialized << std::endl;
    } else {
        std::cerr << "Failed to serialize LoginRequest." << std::endl;
        return 1;
    }
    
    // LoginResponse response;
    // response.ParseFromString(serialized);
    
    // std::cout << "Username: " << response.username() << std::endl;
    // std::cout << "Password: " << response.password() << std::endl;
    
    return 0;
}