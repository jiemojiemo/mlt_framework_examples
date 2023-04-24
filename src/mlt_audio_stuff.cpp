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

void play(const vector<string> &filenames, const string &bgm_filename,
          float bgm_gain) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Playlist video_track;
  for (auto &filename : filenames) {
    Producer producer(profile, NULL, filename.c_str());
    video_track.append(producer);
  }

  Producer bgm_track(profile, NULL, bgm_filename.c_str());
  Filter volume(profile, "volume");
  volume.set("level", bgm_gain);
  bgm_track.attach(volume);

  Tractor tractor(profile);
  tractor.set_track(bgm_track, 0);
  tractor.set_track(video_track, 1);

  Transition audio_mixer(profile, "mix");
  audio_mixer.set("out", 9999);
  tractor.plant_transition(audio_mixer, 0, 1);

  // Create a consumer and connect it to the playlist
  Consumer consumer(profile, nullptr, nullptr);

  // Start playing
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
    cxxopts::Options options("mul_audio_stuff", "MLT audio stuff");
    options.add_options()("f,filenames", "Files to play",
                          cxxopts::value<vector<string>>())(
        "b,bgm", "BGM filename", cxxopts::value<string>())(
        "g,gain", "bgm gain", cxxopts::value<float>());

    auto result = options.parse(argc, argv);
    vector<string> filenames;
    string bgm_filename;
    float gain = 0.5;

    if (result.count("filenames")) {
      filenames = result["filenames"].as<vector<string>>();
    }
    if (result.count("bgm")) {
      bgm_filename = result["bgm"].as<string>();
    }
    if (result.count("gain")) {
      gain = result["gain"].as<float>();
    }

    play(filenames, bgm_filename, gain);
  } catch (const cxxopts::exceptions::exception &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

  Factory::close();
  return 0;
}
