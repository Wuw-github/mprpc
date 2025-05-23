#pragma once
#include "google/protobuf/service.h"
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <google/protobuf/descriptor.h>

class RpcProvider
{
private:
    // registered service information
    struct ServiceInfo
    {
        google::protobuf::Service *m_service;                                              // service object
        std::unordered_map<std::string, google::protobuf::MethodDescriptor *> m_methodMap; // method name
    };

public:
    // Notify the service to the rpc framework
    void NotifyService(google::protobuf::Service *service);

    // Run the rpc service
    void Run();

private:
    // socket connection callback
    void onConnection(const muduo::net::TcpConnectionPtr &conn);

    // message callback for connected socket
    void onMessage(const muduo::net::TcpConnectionPtr &conn,
                   muduo::n et::Buffer *buffer,
                   muduo::Timestamp time);
    // closure callback, used to serialize response and send it back
    void sendRpcResponse(const muduo::net::TcpConnectionPtr &conn,
                         google::protobuf::Message *response);

private:
    muduo::net::EventLoop m_eventLoop;

    // registered services
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
};