cmake_minimum_required(VERSION 2.8)
project(test_dialog)

set(_naoqi_include
  /home/mingfeisun/studyInHKUST/research/2016/projects/footing/NaoRobot/software/naoqi-SDK/naoqi-sdk-2.1.4.13-linux32/include)

set(_naoqi_lib
  /home/mingfeisun/studyInHKUST/research/2016/projects/footing/NaoRobot/software/naoqi-SDK/naoqi-sdk-2.1.4.13-linux32/lib)

include_directories(
    ${INCLUDE_PREFIX}
    ${Boost_INCLUDE_DIRS}
    ${_naoqi_include}
)

link_directories(${_naoqi_lib})

set(_srcs
    exec_shell.h
    exec_shell.cpp
    tulingmodule.h
    tulingmodule.cpp
    dialogtext.h
    dialogtext.cpp
    grabcafe.h
    grabcafe.cpp
    converter.cpp
    converter.h
    wakeup.cpp
    wakeup.h
    main.cpp
)

add_executable(main ${_srcs})
set_target_properties(main PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32" LINK_SEARCH_START_STATIC 1 LINK_SEARCH_END_STATIC 1)

target_link_libraries(main alcommon alvalue alerror alextractor almath alaudio alvision alproxies althread qi qitype boost_system)
