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

void play(const string &filename0, const string &filename1,
          const string &filename2, const string &luma_path) {
  Profile profile;

  Playlist track0;
  Producer p0(profile, NULL, filename0.c_str());
  Producer p1(profile, NULL, filename1.c_str());

  p0.set("in", 100);
  p0.set("out", 174);
  p1.set("in", 100);

  track0.append(p0);
  track0.blank("99");
  track0.append(p1);

  Playlist track1;
  Producer p2(profile, NULL, filename2.c_str());
  p2.set("in", 100);
  p2.set("out", 249);
  track1.blank("49");
  track1.append(p2);

  Transition luma0(profile, "luma", luma_path.c_str());
  luma0.set("softness", 0.5);
  luma0.set("in", 50);
  luma0.set("out", 74);

  Transition luma1(profile, "luma", luma_path.c_str());
  luma1.set("softness", 0.2);
  luma1.set("in", 175);
  luma1.set("out", 199);
  luma1.set("reverse", 1);

  Tractor tractor;
  tractor.set_track(track0, 0);
  tractor.set_track(track1, 1);
  tractor.plant_transition(luma0, 0, 1);
  tractor.plant_transition(luma1, 0, 1);

  Consumer consumer(profile, NULL, nullptr);

  consumer.set("real_time", 1);
  consumer.set("terminate_on_pause", 1);

  consumer.connect(tractor);
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
    cxxopts::Options options("mlt_clock_in_and_out", "MLT clock in and out");
    options.add_options()("f0", "video filename0", cxxopts::value<string>())(
        "f1", "video filename1", cxxopts::value<string>())(
        "f2", "video filename2", cxxopts::value<string>())(
        "luma", "luma path", cxxopts::value<string>());

    auto result = options.parse(argc, argv);

    string filename0;
    string filename1;
    string filename2;
    string luma_path;
    if (result.count("f0")) {
      filename0 = result["f0"].as<string>();
    }
    if (result.count("f1")) {
      filename1 = result["f1"].as<string>();
    }
    if (result.count("f2")) {
      filename2 = result["f2"].as<string>();
    }
    if (result.count("luma")) {
      luma_path = result["luma"].as<string>();
    }
    if (filename0.empty() || filename1.empty() || filename2.empty() ||
        luma_path.empty()) {
      std::cout << options.help() << std::endl;
      exit(1);
    }

    play(filename0, filename1, filename2, luma_path);
  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  Factory::close();
  return 0;
}