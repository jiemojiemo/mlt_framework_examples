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

void play(const string &video_filename, const string &watermark_filename) {
  Profile profile;

  Producer producer(profile, NULL, video_filename.c_str());
  Consumer consumer(profile, NULL, nullptr);
  Filter watermark(profile, "watermark", watermark_filename.c_str());
  watermark.set("composite.geometry", "0=10%/10%:20%x20%; 33=30%/70%:25%x25%; 66=70%/30%:15%x15%; -1=70%/70%:20%x20%");
  watermark.set("composite.out", 100);
  watermark.set("composite.sliced_composite", 1);
  producer.attach(watermark);

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
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-asan/out/lib/mlt");

  try {
    cxxopts::Options options("mlt_bouncy", "MLT bouncy");
    options.add_options()("f,filename", "video filename",
                          cxxopts::value<string>())(
        "w,watermark", "watermark filename", cxxopts::value<string>());

    auto result = options.parse(argc, argv);

    string video_filename;
    if (result.count("filename")) {
      video_filename = result["filename"].as<string>();
    }

    string watermark_filename;
    if (result.count("watermark")) {
      watermark_filename = result["watermark"].as<string>();
    }

    if (video_filename.empty() || watermark_filename.empty()) {
      std::cerr << "input file path invalid\n";
      return -1;
    }

    play(video_filename, watermark_filename);
  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  Factory::close();
  return 0;
}