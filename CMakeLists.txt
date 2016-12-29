cmake_minimum_required(VERSION 3.5)
project(qbttools)

list(APPEND CMAKE_MODULE_PATH ${qbttools_SOURCE_DIR}/cmake/Modules)

find_package(LibtorrentRasterbar REQUIRED)

add_executable(dump-fastresume dump-fastresume.cxx)
target_link_libraries(dump-fastresume LibtorrentRasterbar::LibTorrent)


install(TARGETS dump-fastresume RUNTIME DESTINATION bin)
