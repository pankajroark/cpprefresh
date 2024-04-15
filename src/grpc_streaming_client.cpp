#include <grpcpp/grpcpp.h>
#include <iostream>
#include <route_guide.grpc.pb.h>
#include <route_guide.pb.h>

using namespace std;
using routeguide::RouteGuide;
using routeguide::Point;
using routeguide::Feature;
using routeguide::Rectangle;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::Status;



int main(int argc, char* argv[]) {
  routeguide::Rectangle rect;
  Feature feature;
  ClientContext context;

  rect.mutable_hi()->set_latitude(1);
  rect.mutable_hi()->set_longitude(1);
  rect.mutable_lo()->set_latitude(1);
  rect.mutable_lo()->set_longitude(1);

  auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
  auto stub = RouteGuide::NewStub(channel);

  unique_ptr<ClientReader<Feature>> reader(stub->ListFeatures(&context, rect));
  while (reader->Read(&feature)) {
    cout << feature.name() << ":";
    cout << " latitude: " << feature.location().latitude();
    cout << " longitude: " << feature.location().longitude() << endl;
  }

  Status status = reader->Finish();
  if (!status.ok()) {
    cout << "Error: " << status.error_code() << ": " << status.error_message() << endl;
  } else {
    cout << "Finished" << endl;
  }
  return 0;
}