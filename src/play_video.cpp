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

  Producer producer(profile, NULL, filename);
  Consumer consumer(profile, NULL, nullptr); // defaults to sdl

  // Prevent scaling to the profile size.
  // Let the sdl consumer do all scaling.
  consumer.set("rescale", "nearest");
  consumer.set("real_time", 1);

  // Automatically exit at end of file.
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

  consumer.stop();
}

int main(int, char **argv) {
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-debug/out/lib/mlt");
  play(argv[1]);
  Factory::close();
  return 0;
}
