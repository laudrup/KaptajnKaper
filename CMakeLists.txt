cmake_minimum_required(VERSION 3.15)

project(kaper CXX)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)
include(FetchContent)

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

target_compile_features(project_options INTERFACE
  cxx_std_17
)

if(WIN32)
  target_compile_definitions(project_options INTERFACE
    _CRT_SECURE_NO_WARNINGS
  )
endif()

set_project_warnings(project_warnings)
enable_sanitizers(project_options)

FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

add_executable(kaper
  src/GameStateAttack.cpp
  src/GameStateBoard.cpp
  src/GameStateCity.cpp
  src/GameStateSailingCity.cpp
  src/Graphics.cpp
  src/Image.cpp
  src/Kaper.cpp
  src/KaperCanvas.cpp
  src/StateGame.cpp
  src/StateMenu.cpp
)

target_include_directories(kaper PRIVATE
  src/
)

target_link_libraries(kaper PRIVATE
  sfml-graphics
  project_options
  project_warnings
)

