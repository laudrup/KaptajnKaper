cmake_minimum_required(VERSION 3.15)

project(kaper CXX)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Sanitizers.cmake)

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

add_executable(kaper
  src/GameStateSailingCity.cpp
  src/GameStateBoard.cpp
  src/Kaper.cpp
  src/StateGame.cpp
  src/GameStateCity.cpp
  src/StateMenu.cpp
  src/KaperCanvas.cpp
  src/GameStateAttack.cpp
)

target_include_directories(kaper PRIVATE
  src/
)

target_link_libraries(kaper PRIVATE
  project_options
  project_warnings
)

