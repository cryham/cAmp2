cmake_minimum_required (VERSION 3.0)

project(cAmp2)


file(GLOB SOURCES
    source/*.h source/*.cpp
    source/*/*.h source/*/*.cpp
    libs/*.h libs/*.c libs/*.cpp)


add_executable(${PROJECT_NAME} ${SOURCES})


#  C++17
set(CMAKE_CXX_STANDARD 17)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#link_libraries(stdc++fs)
target_link_libraries(${PROJECT_NAME} stdc++fs)


#  bass - sound
set(bass_SOURCE_DIR "../../bass/src")
add_library(bass SHARED IMPORTED)
set_target_properties(bass PROPERTIES IMPORTED_LOCATION ${bass_SOURCE_DIR}/x64/libbass.so)
target_link_libraries(${PROJECT_NAME} bass)


#  SFML - graphics
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-graphics)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} "-lopengl32 -lglu32 -lgdi32")
else()
    target_link_libraries(${PROJECT_NAME} "-lGL")
endif()


#  GTK for NFD (nativefiledialog)
if(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK REQUIRED gtk+-3.0)
	
	include_directories(${GTK_INCLUDE_DIRS})
	link_directories(${GTK_LIBRARY_DIRS})
	add_definitions(${GTK_CFLAGS_OTHER})

	target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})
endif()


