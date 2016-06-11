cmake_minimum_required(VERSION 2.8)
project(test_dialog)

find_package(qibuild)

qi_create_lib(textToSpeech texttospeech.h texttospeech.cpp)
qi_stage_lib(textToSpeech)

qi_create_bin(test_dialog main.cpp)
qi_use_lib(test_dialog ALCOMMON ALPROXIES textToSpeech)

