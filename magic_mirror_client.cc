/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "magicmirror.grpc.pb.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using magicmirror::ImageFlipRequest;
using magicmirror::ImageFlipReply;
using magicmirror::ImageMirror;
using magicmirror::FlipType;

class ImageMirrorClient {
 public:
  explicit ImageMirrorClient(std::shared_ptr<Channel> channel)
      : stub_(ImageMirror::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string FlipImage(const std::string& image, int width, int height, FlipType fliptype) {
    // Data we are sending to the server.
    ImageFlipRequest request;
    request.set_image(image);
    request.set_width(width);
    request.set_height(height);
    request.set_flip_type(fliptype);

    // Container for the data we expect from the server.
    ImageFlipReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The producer-consumer queue we use to communicate asynchronously with the
    // gRPC runtime.
    CompletionQueue cq;

    // Storage for the status of the RPC upon completion.
    Status status;

    // stub_->PrepareAsyncFlipImage() creates an RPC object, returning
    // an instance to store in "call" but does not actually start the RPC
    // Because we are using the asynchronous API, we need to hold on to
    // the "call" instance in order to get updates on the ongoing RPC.
    std::unique_ptr<ClientAsyncResponseReader<ImageFlipReply> > rpc(
        stub_->PrepareAsyncFlipImage(&context, request, &cq));

    // StartCall initiates the RPC call
    rpc->StartCall();

    // Request that, upon completion of the RPC, "reply" be updated with the
    // server's response; "status" with the indication of whether the operation
    // was successful. Tag the request with the integer 1.
    rpc->Finish(&reply, &status, (void*)1);
    void* got_tag;
    bool ok = false;
    // Block until the next result is available in the completion queue "cq".
    // The return value of Next should always be checked. This return value
    // tells us whether there is any kind of event or the cq_ is shutting down.
    GPR_ASSERT(cq.Next(&got_tag, &ok));

    // Verify that the result from "cq" corresponds, by its tag, our previous
    // request.
    GPR_ASSERT(got_tag == (void*)1);
    // ... and that the request was completed successfully. Note that "ok"
    // corresponds solely to the request for updates introduced by Finish().
    GPR_ASSERT(ok);

    // Act upon the status of the actual RPC.
    if (status.ok()) {
      return reply.image();
    } else {
      return "RPC failed";
    }
  }

 private:
  // Out of the passed in Channel comes the stub, stored here, our view of the
  // server's exposed services.
  std::unique_ptr<ImageMirror::Stub> stub_;
};

static void show_usage(std::string name){
  std::cerr << "Usage: " << name << " INPUT_IMAGE DESTINATION [,options]"
            << "\n\nOptions:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-t,--type TYPE\t\tSpecify the type of mirroring: (horizontal, vertical or both)"
            << std::endl;
}

int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint (in this case,
  // localhost at port 50051). We indicate that the channel isn't authenticated
  // (use of InsecureChannelCredentials()).
  ImageMirrorClient mirror(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));

  // Default flip type
  FlipType flip_type = FlipType::HORIZONTAL;

  if (argc <= 2){
    show_usage(argv[0]);
    return 0;
  }

  // LUT for extract arguments
  std::map<std::string, FlipType> ftmap;
  ftmap["horizontal"] = FlipType::HORIZONTAL;
  ftmap["vertical"] = FlipType::VERTICAL;
  ftmap["both"] = FlipType::BOTH;

  // Extract arguments
  for (int i = 1; i < argc; ++i) {
    if ((argv[i] == "-t") || (argv[i] == "--type")) {
      if (i + 1 < argc && ftmap.find(argv[i+1]) != ftmap.end()) { // Make sure we aren't at the end of argv!
        flip_type = ftmap[ argv[i+1] ]; 
        i++; // Increment 'i' so we don't get the argument as the next argv[i].
      } else { // Uh-oh, there was no argument to the type option.
        std::cerr << "--type option requires one argument." << std::endl;
        return 1;
      }  
    }
  }

  std::string img = argv[1];
  std::string destination_path = argv[2];

  cv::Mat image = cv::imread(img, 0); // Read it grayscale

  if(!image.data){
    std::cout << "Could not open or find the image" << std::endl;
    return -1;
  }

  std::string imgstr = std::string( reinterpret_cast<const char*>(image.data ) );
  std::string reply = mirror.FlipImage( imgstr , image.cols, image.rows, flip_type ); // The actual RPC call!

  cv::Mat flipped = cv::Mat(image.rows, image.cols, CV_8UC1, &reply[0u]);
  imwrite(destination_path, flipped);
  std::cout << "Image saved: " << destination_path << std::endl;

  return 0;
}
