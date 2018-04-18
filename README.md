# magic-mirror
Sample client-server application using gRPC to flip an image

# Instalation
To install gRPC on your system, follow the instructions to build from source [here](https://github.com/grpc/grpc/blob/master/INSTALL.md). This also installs the protocol buffer compiler `protoc` (if you don't have it already), and the C++ gRPC plugin for `protoc`.

## Client and server implementations

The client implementation is at  [magic_mirror_client.cc](https://github.com/heekinho/magic-mirror/blob/master/magic_mirror_client.cc).

The server implementation is at  [magic_mirror_server.cc](https://github.com/heekinho/magic-mirror/blob/master/magic_mirror_server.cc).

### Try it!

Build client and server:

    $ make
Run the server, which will listen on port 50051:

    $ ./magic_mirror_server

Run the client (in a different terminal):

    $ ./magic_mirror_client

If things go smoothly, the client will send "This text represents an image to be flipped" and  you will see the "Image flipped received: deppilf eb ot egami na stneserper txet sihT" in the client side output. 

*Note: Sure this is a temporary output but we can test the skeleton (based on the [gRPC Basics: C++](https://github.com/grpc/grpc/blob/master/examples/cpp/cpptutorial.md) of the client-server architecture before we implement the complete system* 
