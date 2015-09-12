cmake_minimum_required (VERSION 3.3.1)

project(zodiac)

find_package(SDL REQUIRED sdl)
find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
find_package(OpenGL REQUIRED)

set(src
  zodiac.cpp Utils.cpp ShaderProgram.cpp SimpleText.cpp Mhvl.cpp KeyEventManager.cpp EventDispatch.cpp SceneDrawer.cpp ParticleSystem.cpp
  # stegu code
  simplexnoise1234.cpp
  # glee
  external/glee/glee.c)

add_executable(zodiac ${src})
target_compile_definitions(zodiac PRIVATE USE_PROGRAM_OPTIONS)
target_include_directories(zodiac PRIVATE
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/external
  ${SDL_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR})
target_link_libraries(zodiac
  ${SDL_LIBRARIES}
  ${OpenGL_LIBRARY_DIRS}
  ${OpenGL_INCLUDE_DIR})
