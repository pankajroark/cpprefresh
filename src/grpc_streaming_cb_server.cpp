#include <grpcpp/grpcpp.h>
#include <iostream>
#include <route_guide.grpc.pb.h>
#include <route_guide.pb.h>
#include <chrono>
#include <thread>
#include <grpc/support/log.h>

using namespace std;
using routeguide::RouteGuide;
using routeguide::Point;
using routeguide::Feature;
using routeguide::Rectangle;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::CallbackServerContext;
using grpc::Status;
using grpc::ServerWriter;
using grpc::ServerReader;


class RouteGuideImpl final : public RouteGuide::CallbackService {
  public:
    explicit RouteGuideImpl() {}

    grpc::ServerWriteReactor<Feature>* ListFeatures(CallbackServerContext* context, const routeguide::Rectangle* rectangle) override {
      class Lister : public grpc::ServerWriteReactor<Feature> {
        public:
          Lister() : cnt(0){
            Feature f;
            f.set_name("Feature");
            f.mutable_location()->set_latitude(1);
            f.mutable_location()->set_longitude(1);
            feature = f;
            NextWrite();
          }
          void OnDone() override { 
            std::cout << "OnDone" << std::endl;
            delete this; 
          }
          void OnWriteDone(bool) override { 
            std::cout << "OnWriteDone" << std::endl;
            NextWrite(); 
          }
        private:
          void NextWrite() {
            std::cout << "NextWrite" << std::endl;
            if (cnt < 4) {
              StartWrite(&feature);
              std::cout << "written feature" << std::endl;
              cnt++;
              std::cout << "new count " << cnt << std::endl;
              return;
            }
            std::cout << "calling finish" << std::endl;
            Finish(grpc::Status::OK);
          }

          int cnt;
          Feature feature;
      };

      return new Lister();
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
  gpr_set_log_verbosity(GPR_LOG_SEVERITY_ERROR);
  gpr_log(GPR_ERROR, "Starting server");
  RunServer();
  return 0;
}