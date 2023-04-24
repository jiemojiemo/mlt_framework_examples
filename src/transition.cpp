//
// Created by user on 4/8/23.
//
#include <Mlt.h>
#include <thread>
#if defined(__cplusplus)
extern "C" {
#endif

#include <SDL2/SDL.h>

#if defined(__cplusplus)
};
#endif

using namespace Mlt;
using namespace std;

void transition(const char *filename, const char *filename2) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Producer producer0(profile, "avformat", filename);
  Producer producer1(profile, "avformat", filename2);
  producer0.set("in", 0);
  producer0.set("out", 100);
  producer1.set("in", 0);
  producer1.set("out", 200);

  Consumer consumer(profile, "sdl2", nullptr); // defaults to sdl
  consumer.set("rescale", "nearest");
  consumer.set("real_time", 1);
  consumer.set("terminate_on_pause", 1);

  Filter audio_converter(profile, "audioconvert");
  Filter swscale_filter(profile, "rescale");
  Filter swscale_filter1(profile, "rescale");

  // add transition
  Tractor tractor;
  tractor.set_track(producer0, 0);
  tractor.plant_filter(swscale_filter, 0);

  tractor.set_track(producer1, 1);
  tractor.plant_filter(swscale_filter1, 1);
  //  Transition t(profile, "luma");
  //  t.set("in", 50);
  //  t.set("out", 100);
  //  tractor.plant_transition(t, 0, 1);

  consumer.connect(tractor);
  //  tractor.connect(audio_converter);
  //  audio_converter.connect(swscale_filter);
  //  swscale_filter.connect(producer0);

  consumer.start();

  while (!consumer.is_stopped()) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        producer0.set("done", 1);
        producer1.set("done", 1);
        consumer.stop();
        break;
      }
    }
  }

  consumer.stop();
}

int main(int, char **argv) {
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-debug/out/lib/mlt");
  transition(argv[1], argv[2]);
  Factory::close();
  return 0;
}
