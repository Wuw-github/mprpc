# MPRPC - A Simple RPC Framework

MPRPC is a C++ RPC (Remote Procedure Call) framework designed to facilitate communication between distributed services. It leverages Google Protocol Buffers for data serialization, the Muduo network library for asynchronous I/O, and Apache Zookeeper for service registration and discovery.

## Features

*   **Protocol Buffers:** Uses Google Protocol Buffers for defining services and message types, ensuring efficient and type-safe data exchange.
*   **Service Discovery with Zookeeper:** Integrates with Apache Zookeeper, allowing services (callees) to register themselves and clients (callers) to discover available services dynamically.
*   **Asynchronous Networking with Muduo:** Employs the Muduo C++ network library for high-performance, non-blocking I/O operations on the server-side.
*   **Client-Side Channel:** Provides an `MprpcChannel` for clients to make RPC calls.
*   **Server-Side Provider:** Includes an `RpcProvider` to host and manage RPC services.
*   **Configuration Management:** Simple configuration file loading for server and Zookeeper details.
*   **Basic Logging:** Includes a simple asynchronous logger.
*   **Example Services:** Comes with `UserService` and `FriendService` examples to demonstrate usage.

## Dependencies

*   **Google Protocol Buffers (protobuf)**: For IDL and serialization.
*   **Muduo C++ Network Library**: For server-side networking.
*   **Apache Zookeeper C Client Library**: For service registration and discovery.
*   **CMake** (or your preferred build system): For building the project.
*   A C++11 compliant compiler (e.g., g++).


