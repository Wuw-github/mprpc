#pragma once
#include "google/protobuf/service.h"

class RpcProvider {

  public:
    // Notify the service to the rpc framework
    void NotifyService(google::protobuf::Service* service);

    // Run the rpc service
    void Run();

  private:
    // The thread pool for processing rpc requests
    ThreadPool threadpool_;
};