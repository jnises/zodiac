cmake_minimum_required (VERSION 3.3.1)

project(zodiac)

INCLUDE(FindPkgConfig)
find_package(sdl2 REQUIRED)
find_package(Boost 1.63 COMPONENTS program_options REQUIRED)
find_package(OPENGL REQUIRED)
find_library(COREFOUNDATION CoreFoundation)

set(src
  zodiac.cpp Utils.cpp ShaderProgram.cpp SimpleText.cpp Mhvl.cpp KeyEventManager.cpp EventDispatch.cpp SceneDrawer.cpp ParticleSystem.cpp
  Utils.h ShaderProgram.h SimpleText.h Mhvl.h KeyEventManager.h EventDispatch.h SceneDrawer.h ParticleSystem.h
  # stegu code
  simplexnoise1234.cpp
  simplexnoise1234.h
  # glee
  external/glee/glee.c
  external/glee/glee.h
)

add_executable(zodiac ${src})
target_compile_definitions(zodiac PRIVATE USE_PROGRAM_OPTIONS)
target_include_directories(zodiac PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/external
  ${SDL2_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  )
target_link_libraries(zodiac
  ${SDL2_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${Boost_LIBRARIES}
  ${COREFOUNDATION}
  )
set_property(TARGET zodiac PROPERTY CXX_STANDARD 14)
