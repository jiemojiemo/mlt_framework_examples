find_package(PkgConfig REQUIRED)
pkg_check_modules(sdl2 IMPORTED_TARGET sdl2)

function(add_executable_and_install target_name source_files)
    # 添加可执行文件
    add_executable(${target_name} ${source_files})
    target_link_libraries(${target_name} PRIVATE mlt++ PkgConfig::sdl2 )

    # 将可执行文件添加到安装规则
    install(TARGETS ${target_name}
            RUNTIME DESTINATION bin)
endfunction()

add_executable_and_install(mlt_compile mlt_compile.cpp)
add_executable_and_install(mlt_all mlt_all.cpp)
add_executable_and_install(mlt_audio_stuff mlt_audio_stuff.cpp)
add_executable_and_install(mlt_avantika_title mlt_avantika_title.cpp)
add_executable_and_install(mlt_bouncy mlt_bouncy.cpp)
add_executable_and_install(mlt_clock_in_and_out mlt_clock_in_and_out.cpp)
add_executable_and_install(mlt_composite_transition mlt_composite_transition.cpp)
add_executable_and_install(mlt_effect_in_middle mlt_effect_in_middle.cpp)
