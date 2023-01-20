cmake_minimum_required(VERSION 3.20)

project(MuchCool-EFI)

set (CMAKE_C_STANDARD 17)
set (CMAKE_CXX_STANDARD 20)

add_library(muchcool_efi
  source/efi.cpp
)

target_include_directories(muchcool_efi
PUBLIC
  include
)