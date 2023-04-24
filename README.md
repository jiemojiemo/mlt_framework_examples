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