# MLT Framework API Examples

Introduction:
Welcome to this repository showcasing a collection of examples using the MLT Framework API. The MLT Framework is a powerful multimedia processing engine that enables developers to create rich and diverse multimedia applications. This repository aims to provide a comprehensive understanding of the framework's capabilities through practical and easy-to-understand examples.

The examples in this repository demonstrate a wide range of functionalities, including:

+ mlt_all: A program that plays multiple video files simultaneously, showcasing MLT's ability to manage and process multiple video streams.
+ mlt_effect_in_middle: This example demonstrates how to apply various video effects to a clip, such as transitions, color correction, and compositing, to create visually appealing video content.
+ mlt_bouncy: An example that implements a Picture-in-Picture (PiP) effect, allowing you to display one video inside another, creating a dynamic viewing experience.

These examples serve as an excellent starting point for developers looking to integrate the MLT Framework into their multimedia projects. We hope that you find these examples helpful in understanding the MLT Framework and that they inspire you to create your own exciting multimedia applications. Happy coding!

# Build
```bash
git clone https://github.com/jiemojiemo/mlt_framework_examples.git
cd mlt_framework_examples
git submodule update --init --recursive
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=build/install
cmake --build build --target install
```
MLT modules are installed in `build/install/lib/mlt/` and `build/install/share/mlt/` after build.

# Run
1. Set environments
2. Run demos
for example in my mac:
```bash
export MLT_DATA=/Users/user/Documents/develop/mlt_framework_examples/build/install/share/mlt
export MLT_REPOSITORY=/Users/user/Documents/develop/mlt_framework_examples/build/install/lib/mlt
export DYLD_LIBRARY_PATH="/Users/user/Documents/develop/mlt_framework_examples/build/install/lib:$DYLD_LIBRARY_PATH"
cd build/install/bin
./mlt_effect_in_middle -i /path/to/video.mp4 
```
