cmake_minimum_required (VERSION 3.0)

project(cAmp2)

#  library for graphics and window - pick one
option(USE_SFML  "Build with graphics using SFML."  ON)
option(USE_BGFX  "Build with graphics using BGFX and SDL2."  OFF)


file(GLOB SOURCES
    source/*.h source/*.cpp
    libs/*.h libs/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})


#  C++17
set(CMAKE_CXX_STANDARD 17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#link_libraries(stdc++fs)
target_link_libraries(${PROJECT_NAME} stdc++fs)


#  bass - sound
set(bass_SOURCE_DIR "../../bass/src")
ADD_LIBRARY(bass SHARED IMPORTED)
SET_TARGET_PROPERTIES(bass PROPERTIES IMPORTED_LOCATION ${bass_SOURCE_DIR}/x64/libbass.so)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} bass)


#  SFML - graphics
if (USE_SFML)
    find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
    target_link_libraries(${PROJECT_NAME} sfml-graphics)
endif(USE_SFML)

#  BGFX - graphics
if (USE_BGFX)
    
endif(USE_BGFX)
