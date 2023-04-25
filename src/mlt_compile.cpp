//
// Created by user on 3/28/23.
//
#include "cxxopts.hpp"
#include <Mlt.h>
#include <iostream>
#if defined(__cplusplus)
extern "C" {
#endif

#include <SDL2/SDL.h>

#if defined(__cplusplus)
}
#endif
using namespace Mlt;
using namespace std;

void compile(const char *in_path, const char *out_path) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Producer producer(profile, "loader", in_path);
  Consumer consumer(profile, "avformat", out_path);

  consumer.connect(producer);

  consumer.run();
  consumer.stop();
}

int main(int argc, char **argv) {
  char *mlt_repository = getenv("MLT_REPOSITORY");
  if (mlt_repository == nullptr) {
    std::cout << "MLT_REPOSITORY is not set" << std::endl;
    exit(1);
  }

  char *mlt_data = getenv("MLT_DATA");
  if (mlt_data == nullptr) {
    std::cout << "MLT_DATA is not set" << std::endl;
    exit(1);
  }

  Factory::init(mlt_repository);

  try {
    cxxopts::Options options("mlt_compile", "Compile video");
    options.add_options()("i,input", "video filename0",
                          cxxopts::value<string>())(
        "o,output", "video filename1", cxxopts::value<string>());

    auto result = options.parse(argc, argv);
    std::string input;
    std::string output;

    if (result.count("input")) {
      input = result["input"].as<std::string>();
    }
    if (result.count("output")) {
      output = result["output"].as<std::string>();
    }

    if (input.empty() || output.empty()) {
      std::cout << options.help() << std::endl;
      exit(1);
    }

    compile(input.c_str(), output.c_str());

  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  Factory::close();
  return 0;
}
