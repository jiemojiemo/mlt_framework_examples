//
// Created by user on 4/24/23.
//
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

void play(const string &filename0, const string &filename1) {
  Profile profile;

  Producer track0(profile, NULL, filename0.c_str());
  track0.set("out", 74);

  Playlist track1;
  Producer p1(profile, NULL, filename1.c_str());
  track1.blank("49");
  track1.append(p1);

  Transition t0(profile, "composite", "0=57%/10%:33%x33%; -1=0%/0%:100%x100%");
  t0.set("progressive", 1);
  t0.set("distort", true);
  t0.set("in", 50);
  t0.set("out", 74);
  t0.set("sliced_composite", 1);
  t0.set("a_track", 0);
  t0.set("b_track", 1);

  Transition t1(profile, "mix", "-1");
  t1.set("in", 50);
  t1.set("out", 74);
  t1.set("a_track", 0);
  t1.set("b_track", 1);

  Tractor tractor;
  tractor.set_track(track0, 0);
  tractor.set_track(track1, 1);
  tractor.plant_transition(t0, 0, 1);
  tractor.plant_transition(t1, 0, 1);

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
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-asan/out/lib/mlt");

  try {
    cxxopts::Options options("mlt_composite_transition",
                             "MLT composite transition");
    options.add_options()("f0", "video filename0", cxxopts::value<string>())(
        "f1", "video filename1", cxxopts::value<string>());

    auto result = options.parse(argc, argv);

    string filename0;
    string filename1;
    if (result.count("f0")) {
      filename0 = result["f0"].as<string>();
    }
    if (result.count("f1")) {
      filename1 = result["f1"].as<string>();
    }

    if (filename0.empty() || filename1.empty()) {
      std::cout << options.help() << std::endl;
      exit(1);
    }

    play(filename0, filename1);
  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  Factory::close();
  return 0;
}