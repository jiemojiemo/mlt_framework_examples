//
// Created by user on 3/14/23.
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

  Producer producer(profile, "count");
  Consumer consumer(profile, "sdl2", nullptr); // defaults to sdl

  consumer.set("rescale", "nearest");
  consumer.set("realtime", 1);

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
