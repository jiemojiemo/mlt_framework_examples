//
// Created by user on 4/19/23.
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

void play(const char *filename) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Producer track0(profile, NULL, filename);
  Producer track1(profile, "loader", ".mpl");
  auto length = track0.get_playtime();
  track1.set("text", "Hello\nWorld");
  track1.set("in", 0);
  track1.set("out", length - 1);
  track1.set("length", length);
  //  track1.set("a_track", 0);
  //  track1.set("b_track", 1);

  Transition transition(profile, "composite", "10%/10%:15%x15%");
  transition.set("in", 100);
  transition.set("out", length - 1);

  Consumer consumer(profile, NULL, nullptr); // defaults to sdl

  Tractor tractor(profile);
  auto multitrack = std::shared_ptr<Multitrack>(tractor.multitrack());
  auto field = std::shared_ptr<Field>(tractor.field());
  multitrack->connect(track0, 0);
  multitrack->connect(track1, 1);
  field->plant_transition(transition, 0, 1);

  // Prevent scaling to the profile size.
  // Let the sdl consumer do all scaling.
  consumer.set("rescale", "nearest");
  consumer.set("real_time", 1);

  // Automatically exit at end of file.
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

int main(int, char **argv) {
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-asan/out/lib/mlt");
  play(argv[1]);
  Factory::close();
  return 0;
}
