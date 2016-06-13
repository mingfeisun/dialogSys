cmake_minimum_required(VERSION 2.8)
project(test_dialog)

find_package(qibuild)
set(_srcs
    texttospeech.cpp
    texttospeech.h
    speechtotext.cpp
    speechtotext.h
    main.cpp)

qi_create_bin(test_dialog ${_srcs})
qi_use_lib(test_dialog ALCOMMON ALPROXIES)

