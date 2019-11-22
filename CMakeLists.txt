cmake_minimum_required (VERSION 3.0)

project(cAmp2)

#option(BUILD_TEST  "Build test."    ON)

#add_subdirectory(source)
file(GLOB SOURCES source/*.h source/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

#  C++20
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS OFF)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

#link_libraries(stdc++fs)
target_link_libraries(${PROJECT_NAME} stdc++fs)


#  bass - sound
set(bass_SOURCE_DIR "../../bass/src")
ADD_LIBRARY(bass SHARED IMPORTED)
SET_TARGET_PROPERTIES(bass PROPERTIES IMPORTED_LOCATION ${bass_SOURCE_DIR}/x64/libbass.so)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} bass)


#  SFML - graphics
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-graphics)

