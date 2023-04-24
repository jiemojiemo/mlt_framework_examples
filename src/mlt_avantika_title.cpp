//
// Created by user on 4/23/23.
//
#include "cxxopts.hpp"
#include <Mlt.h>
#include <iostream>
#if defined(__cplusplus)
extern "C" {
#endif

#include <SDL2/SDL.h>

#if defined(__cplusplus)
};
#endif

using namespace Mlt;
using namespace std;

void play(const string &pango_file_path) {
  Profile profile;
  Producer producer(profile, NULL, pango_file_path.c_str());
  Consumer consumer(profile, NULL, nullptr);

  consumer.set("real_time", 1);
  consumer.set("terminate_on_pause", 1);

  consumer.connect(producer);
  consumer.start();

  while (!consumer.is_stopped()) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        producer.set("done", 1);
        consumer.stop();
        break;
      }
    }
  }
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
    cxxopts::Options options("mlt_avantika_title", "MLT avantika title");
    options.add_options()("f,filename", "Pango filename",
                          cxxopts::value<string>());

    auto result = options.parse(argc, argv);

    string pango_file_path;
    if (result.count("filename")) {
      pango_file_path = result["filename"].as<string>();
    }

    if (pango_file_path.empty()) {
      std::cerr << "pango file path invalid\n";
      return -1;
    }

    play(pango_file_path);

  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  Factory::close();
  return 0;
}