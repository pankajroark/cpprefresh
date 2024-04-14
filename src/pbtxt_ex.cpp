#include <cxxopts.hpp>
#include <iostream>
#include <config.pb.h>

using namespace std;

int main(int argc, char* argv[]) {
  string confPath;

  cxxopts::Options options("cxxopts_example", "Test cxxopts");
  options.add_options()("c,config", "Path to config file", cxxopts::value<string>(confPath));

  auto result = options.parse(argc, argv);
  if (result.count("config")) {
    confPath = result["config"].as<string>();
    cout << "Config path: " << confPath << endl;
  }
  return 0;
}