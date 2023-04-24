//
// Created by user on 3/28/23.
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

void compile(const char *in_path, const char* out_path) {
  Profile profile; // defaults to dv_pal
  profile.set_frame_rate(30, 1);
  profile.set_width(640);
  profile.set_height(360);
  profile.set_progressive(1);
  profile.set_sample_aspect(1, 1);
  profile.set_display_aspect(16, 9);
  profile.set_colorspace(709);

  Producer producer(profile, "avformat", in_path);
  Consumer consumer(profile, "avformat", out_path);
  Filter audio_converter(profile, "audioconvert");
  Filter swscale_filter(profile, "rescale");

  // Prevent scaling to the profile size.
  // Let the sdl consumer do all scaling.
  consumer.set("rescale", "nearest");

  // Automatically exit at end of file.
  consumer.set("terminate_on_pause", 1);
  consumer.set("frequency", 44100);

  consumer.connect(audio_converter);
  audio_converter.connect(swscale_filter);
  swscale_filter.connect(producer);

  consumer.run();
  consumer.stop();
}


int main(int, char **argv) {
  Factory::init(
      "/Users/user/Documents/develop/mlt/cmake-build-debug/out/lib/mlt");
  compile(argv[1], argv[2]);
  Factory::close();
  return 0;
}
