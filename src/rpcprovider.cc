#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
#include "logger.h"
#include <functional>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/service.h"

std::unique_ptr<muduo::net::TcpServer> tcpserver;

void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo info;

    const google::protobuf::ServiceDescriptor *pServiceDesc = service->GetDescriptor();
    std::string serviceName = pServiceDesc->name();

    // get number of methods of the service
    int methodCount = pServiceDesc->method_count();

    std::cout << "serviceName: " << serviceName << std::endl;
    LOG_INFO("serviceName: %s", serviceName.c_str());

    for (int i = 0; i < methodCount; ++i)
    {
        const google::protobuf::MethodDescriptor *pMethodDesc = pServiceDesc->method(i);
        std::string methodName = pMethodDesc->name();

        info.m_methodMap.insert({fullMethodName, pMethodDesc});
        LOG_INFO("methodName: %s", methodName.c_str());
    }
    info.m_service = service;
    m_serviceMap.insert({serviceName, info});
}

void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetConfig().load("rpcserverip");
    uint64_t port = std::stoi(MprpcApplication::GetConfig().load("rpcserverport"));
    muduo::net::InetAddress address(ip, port);

    // create a TcpServer object
    muduo::net::TcpServer server(&m_eventLoop, address, "RpcProvider");

    // binding the message callback
    server.setConnectionCallback(std::bind(&RpcProvider::onConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    // restrict the thread number
    server.setThreadNum(4);

    // start the server
    server.start();
    std::cout << "RpcProvider started on " << ip << ":" << port << std::endl;

    m_eventLoop.loop();
}

void RpcProvider::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    if (!conn->connected())
    {
        conn->shutdown();
    }
}

/*
    RpcProvider and RpcConsumer needs to agree with the protobuf type
    header_size (4 bytes) + header + args
*/
// if an remote procedure call is received, the server will call this function
void RpcProvider::onMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp time)
{
    std::string receiveBuf = buffer->retrieveAllAsString();

    // read first 4 bytes for the header size
    uint32_t headerSize = 0;
    receiveBuf.copy((char *)&headerSize, 4, 0);

    // based on header size, read RpcHeader
    std::string rpc_header_str = receiveBuf.substr(4, headerSize);
    mprpc::RpcHeader rpc_header;

    std::string serviceName;
    std::string methodName;
    uint32_t argSize = 0;
    if (rpc_header.ParseFromString(rpc_header_str))
    {
        serviceName = rpc_header.service_name();
        methodName = rpc_header.method_name();
        argSize = rpc_header.args_size();
    }
    else
    {
        // header parse error
        std::cout << "RpcHeader ParseFromString error!" << std::endl;
        return;
    }

    std::string args_str = receiveBuf.substr(4 + headerSize, argSize);

    // find the service and method object

    auto it = m_serviceMap.find(serviceName);
    if (it == m_serviceMap.end())
    {
        std::cout << "Service not found: " << serviceName << std::endl;
        return;
    }

    ServiceInfo &serviceInfo = it->second;
    google::protobuf::Service *service = serviceInfo.m_service;

    auto itMethod = serviceInfo.m_methodMap.find(methodName);
    if (itMethod == serviceInfo.m_methodMap.end())
    {
        std::cout << "Method not found: " << methodName << std::endl;
        return;
    }

    const google::protobuf::MethodDescriptor *method = itMethod->second;

    // create request and response object
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if (!request->ParseFromString(args_str))
    {
        std::cout << "Request parse error!" << std::endl;
        return;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    // create a closure object callback
    google::protobuf::Closure *done =
        google::protobuf::NewCallback<RpcProvider,
                                      const muduo::net::TcpConnectionPtr &,
                                      google::protobuf::Message *>(this, &RpcProvider::sendRpcResponse, conn, response);

    // based on rpc message, call the method
    service->CallMethod(method, nullptr, request, response, done);
}

void RpcProvider::sendRpcResponse(const muduo::net::TcpConnectionPtr &conn,
                                  google::protobuf::Message *response)
{
    // serialize response
    std::string response_str;
    if (response->SerializeToString(&response_str))
    {
        // send response
        conn->send(response_str);
    }
    else
    {
        std::cout << "Response serialize error!" << std::endl;
    }

    // mimic http response short connection,
    // close the connection after sending response
    conn->shutdown();
}