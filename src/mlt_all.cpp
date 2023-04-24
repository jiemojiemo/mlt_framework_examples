//
// Created by user on 4/6/23.
//
#include "cxxopts.hpp"
#include <Mlt.h>
#include <iostream>
#include <string>
#include <vector>
#if defined(__cplusplus)
extern "C" {
#endif

#include "SDL2/SDL.h"

#if defined(__cplusplus)
}
#endif

using namespace Mlt;

void play(int argc, char *argv[]) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Playlist playlist;
  for (int i = 1; i < argc; i++) {
    Producer producer(profile, NULL, argv[i]);
    playlist.append(producer);
  }

  // Create a consumer and connect it to the playlist
  Consumer consumer(profile, nullptr, nullptr);

  // Start playing
  consumer.set("real_time", 1);
  consumer.set("terminate_on_pause", 1);

  consumer.connect(playlist);
  consumer.start();

  while (!consumer.is_stopped()) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        consumer.stop();
        break;
      }
    }
  }

  consumer.stop();
}

void play(const std::vector<std::string> &filenames) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Playlist playlist;
  for (auto &filename : filenames) {
    Producer producer(profile, NULL, filename.c_str());
    playlist.append(producer);
  }

  // Create a consumer and connect it to the playlist
  Consumer consumer(profile, "sdl2", nullptr);

  // Start playing
  consumer.set("real_time", 1);
  consumer.set("terminate_on_pause", 1);

  consumer.connect(playlist);
  consumer.start();

  while (!consumer.is_stopped()) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        consumer.stop();
        break;
      }
    }
  }

  consumer.stop();
}

int main(int argc, char *argv[]) {
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

  // Initialize MLT
  Factory::init(mlt_repository);

  try {
    cxxopts::Options options("mlt_all", "MLT All");
    options.add_options()("f,filenames", "Files to play",
                          cxxopts::value<std::vector<std::string>>());

    auto result = options.parse(argc, argv);
    if (result.count("filenames")) {
      auto filenames = result["filenames"].as<std::vector<std::string>>();
      play(filenames);
    } else {
      std::cout << "No filenames specified" << std::endl;
    }

  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  // Close and cleanup
  Factory::close();
  return 0;
}
