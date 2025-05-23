#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include "mprpcapplication.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
/*
    header format: header_size + header + args
*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *service = method->service();
    const std::string service_name = service->name();
    const std::string method_name = method->name();

    std::string args_str;
    if (!request->SerializeToString(&args_str))
    {
        std::cout << "request serialize to string failed" << std::endl;
        return;
    }

    uint32_t args_size = args_str.size();

    mprpc::RpcHeader rpc_header;
    rpc_header.set_service_name(service_name);
    rpc_header.set_method_name(method_name);
    rpc_header.set_args_size(args_size);

    std::string rpc_header_str;
    if (!rpc_header.SerializeToString(&rpc_header_str))
    {
        std::cout << "rpc_header serialize to string failed" << std::endl;
        return;
    }

    uint32_t header_size = rpc_header_str.size();

    // concat header and args

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char *)&header_size, 4));
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;

    // send to socket using tcp connection
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        std::cout << "socket error, errno: " << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    std::string ip = MprpcApplication::GetConfig().load("rpcserverip");
    uint64_t port = std::stoi(MprpcApplication::GetConfig().load("rpcserverport"));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // connect to server
    if (-1 == connect(clientfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        std::cout << "connect error, errno: " << errno << std::endl;
        close(clientfd);
        exit(EXIT_FAILURE);
    }

    // send data
    ssize_t send_size = send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0);
    if (send_size == -1)
    {
        std::cout << "send error, errno: " << errno << std::endl;
        close(clientfd);
        return;
    }

    // receive response
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, sizeof(recv_buf), 0)))
    {
        std::cout << "recv error, errno: " << errno << std::endl;
        close(clientfd);
        return;
    }

    // deserialize response
    // if recv_buf has \0, it will be truncated
    // std::string response_str(recv_buf, 0, recv_size);
    if (!response->ParseFromArray(recv_buf, recv_size))
    {
        std::cout << "response parse error!" << std::endl;
        close(clientfd);
        return;
    }

    close(clientfd);
}