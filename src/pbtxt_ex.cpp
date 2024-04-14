#include <cxxopts.hpp>
#include <iostream>
#include <config.pb.h>
#include <fstream>
#include <google/protobuf/io/zero_copy_stream_impl.h> // Include ZeroCopyInputStream
#include <google/protobuf/text_format.h>

using namespace google::protobuf::io;
using namespace google::protobuf;


using namespace std;

int main(int argc, char* argv[]) {
  string confPath;

  cxxopts::Options options("cxxopts_example", "Test cxxopts");
  options.add_options()("c,config", "Path to config pbtxt file", cxxopts::value<string>(confPath));

  auto result = options.parse(argc, argv);
  if (result.count("config")) {
    confPath = result["config"].as<string>();
    cout << "Config path: " << confPath << endl;
  } else {
    cout << "Config path not provided" << endl;
    return 1;
  }

  ifstream input(confPath);
  if (!input) {
    cerr << "Failed to open " << confPath << endl;
    return 1;
  }

  Config config;
  IstreamInputStream zero_copy_input(&input);
   // Parse the .pbtxt file into the Config message
  if (!TextFormat::Parse(&zero_copy_input, &config)) {
      cerr << "Failed to parse " << confPath << endl;
      return 1;
  }

  cout << "Config: " << config.key() << ":" << config.value() << endl;

  return 0;
}