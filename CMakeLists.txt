cmake_minimum_required(VERSION 3.5)
project(eNet VERSION 0.1.0 LANGUAGES C CXX)

if($ENV{eNet_BUILD_EXAMPLES})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif()

add_subdirectory(thirdparty/eLog)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS  "libBuild/src/*.cpp")

if(NOT TARGET eNet)
    add_library(eNet SHARED ${SOURCE_FILES})
    target_compile_features(eNet PRIVATE cxx_std_20)
    target_link_libraries(eNet PRIVATE eLogLib)
    if(WIN32)
        target_link_libraries(eNet PRIVATE ws2_32)
    endif()
    target_include_directories(eNet PUBLIC libBuild/hdr)
    target_include_directories(eNet PRIVATE thirdparty/eLog/libBuild/hdr)
endif()

message(STATUS "eNet build examples: " $ENV{eNet_BUILD_EXAMPLES})

if($ENV{eCMD_BUILD_EXAMPLES})
    add_subdirectory(buildtest)
endif()