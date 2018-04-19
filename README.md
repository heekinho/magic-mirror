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

    $ ./magic_mirror_client lena.png output.png [--type (horizontal|vertical|both)]

If things go smoothly, the client will save a flipped grayscale image in the specified path.
