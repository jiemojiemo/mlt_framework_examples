//
// Created by user on 4/6/23.
//
#include <Mlt.h>
#include <iostream>
#include <thread>
#if defined(__cplusplus)
extern "C" {
#endif

#include <SDL2/SDL.h>

#if defined(__cplusplus)
};
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

int main(int argc, char *argv[]) {

  // Initialize MLT
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-debug/out/lib/mlt");

  play(argc, argv);

  // Close and cleanup
  Factory::close();
  return 0;
}
