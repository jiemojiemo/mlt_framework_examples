
find_package(SDL2 REQUIRED)
function(add_executable_and_install target_name source_files)
    # 添加可执行文件
    add_executable(${target_name} ${source_files})
    target_link_libraries(${target_name} PRIVATE mlt++ SDL2::SDL2)

    # 将可执行文件添加到安装规则
    install(TARGETS ${target_name}
            RUNTIME DESTINATION bin)
endfunction()

add_executable_and_install(play play_video.cpp)
add_executable_and_install(hello hello.cpp)
add_executable_and_install(producer_count producer_count.cpp)
add_executable_and_install(compile compile.cpp)
add_executable_and_install(effect_in_middle effect_in_middle.cpp)
add_executable_and_install(play_all play_all.cpp)
add_executable_and_install(transition transition.cpp)
add_executable_and_install(multitrack_play multitrack_play.cpp)
add_executable_and_install(watermark watermark.cpp)
add_executable_and_install(mlt_audio_stuff mlt_audio_stuff.cpp)
add_executable_and_install(mlt_avantika_title mlt_avantika_title.cpp)
add_executable_and_install(mlt_bouncy mlt_bouncy.cpp)
add_executable_and_install(mlt_clock_in_and_out mlt_clock_in_and_out.cpp)
add_executable_and_install(mlt_composite_transition mlt_composite_transition.cpp)
