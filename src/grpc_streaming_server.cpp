#include <grpcpp/grpcpp.h>
#include <iostream>
#include <route_guide.grpc.pb.h>
#include <route_guide.pb.h>
#include <chrono>
#include <thread>

using namespace std;
using routeguide::RouteGuide;
using routeguide::Point;
using routeguide::Feature;
using routeguide::Rectangle;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using grpc::ServerReader;


class RouteGuideImpl final : public RouteGuide::Service {
  public:
    explicit RouteGuideImpl() {}

    Status ListFeatures(ServerContext* context, const routeguide::Rectangle* rectangle, ServerWriter<Feature>* writer) override {
      for (int i = 0; i < 10; i++) {
        Feature f;
        f.set_name("Feature");
        f.mutable_location()->set_latitude(i);
        f.mutable_location()->set_longitude(i);
        writer->Write(f);
        this_thread::sleep_for(chrono::seconds(1));
      }
      return Status::OK;
    }
};


void RunServer() {
  string server_address("0.0.0.0:50051");
  RouteGuideImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  unique_ptr<Server> server(builder.BuildAndStart());
  cout << "Server listening on " << server_address << endl;
  server->Wait();
}

int main(int argc, char* argv[]) {
  RunServer();
  return 0;
}