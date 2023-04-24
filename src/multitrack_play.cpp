//
// Created by user on 4/19/23.
//
#include <Mlt.h>
#if defined(__cplusplus)
extern "C" {
#endif

#include <SDL2/SDL.h>

#if defined(__cplusplus)
};
#endif

using namespace Mlt;
using namespace std;

void play() {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  //  const char *file0 = "/Users/user/Downloads/2023_04_13_18_17_IMG_0510.MP4";
  //  const char *file1 = "/Users/user/Downloads/output_video.mp4";
  const char *file0 = "/Users/user/Downloads/video_1280x720_30fps_10sec.mp4";
  Producer producer0(profile, NULL, file0);
  Producer producer1(profile, NULL, file0);
  Consumer consumer(profile, "sdl2", nullptr); // defaults to sdl

  Multitrack multitrack(mlt_multitrack_init());
  multitrack.connect(producer0, 0);
  multitrack.connect(producer1, 1);

  // Prevent scaling to the profile size.
  // Let the sdl consumer do all scaling.
  consumer.set("rescale", "nearest");
  consumer.set("real_time", 1);

  // Automatically exit at end of file.
  consumer.set("terminate_on_pause", 1);
  consumer.connect(multitrack);

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
      "/Users/user/Documents/develop/mlt/cmake-build-debug/out/lib/mlt");
  play();
  Factory::close();
  return 0;
}
