cmake_minimum_required (VERSION 3.8)

project(Ripper2A03Project)

set(srcDir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(incDir ${CMAKE_CURRENT_SOURCE_DIR}/inc)

include(ExternalProject)

# cxxopts dependency
ExternalProject_Add(cxxopts_ext
  PREFIX ${CMAKE_BINARY_DIR}/cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG 9990f73845d76106063536d7cd630ac15cb4a065
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  )
ExternalProject_Get_Property(cxxopts_ext SOURCE_DIR)
set(cxxoptsSourceDir ${SOURCE_DIR})
message("cxxopts source directory: ${cxxoptsSourceDir}")

# fmtlib dependency
ExternalProject_Add(fmtlib_ext
  PREFIX ${CMAKE_BINARY_DIR}/fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9f70b034e1cbd39e6beee8de7cf0bafba8ee4f80
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
  )
ExternalProject_Get_Property(fmtlib_ext SOURCE_DIR)
set(fmtlibSourceDir ${SOURCE_DIR})
message("fmtlib source directory: ${fmtlibSourceDir}")

set(sources 
  ${srcDir}/cl_options.cpp
  ${srcDir}/ines_header.cpp
  ${srcDir}/instruction_data.cpp
  ${srcDir}/instruction_info.cpp
  ${srcDir}/instruction_visitor.cpp
  ${srcDir}/internal_ram.cpp
  ${srcDir}/internal_ram_address_line.cpp
  ${srcDir}/main.cpp
  ${srcDir}/mnemonics.cpp
  ${srcDir}/nes_rom.cpp
  )

add_executable(Ripper2A03 ${sources})
add_library(cxxopts INTERFACE)
add_dependencies(cxxopts cxxopts_ext)
add_library(fmtlib INTERFACE)
add_dependencies(fmtlib fmtlib_ext)
add_dependencies(Ripper2A03 
  cxxopts
  fmtlib
  )
target_include_directories(Ripper2A03 
  PUBLIC ${incDir}
  PRIVATE "${cxxoptsSourceDir}/include"
  PRIVATE "${fmtlibSourceDir}/include"
  )


#add_library(LibRipper2A03 STATIC ${libSources})
#target_include_directories(LibRipper2A03 ${incDir})

#target_link_libraries(Ripper2A03 LibRipper2A03)

install(TARGETS Ripper2A03 DESTINATION bin)

set_property(TARGET Ripper2A03 PROPERTY CXX_STANDARD 17)
set_property(TARGET Ripper2A03 PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
  target_compile_options(Ripper2A03 PUBLIC "/std:c++17")
endif()
