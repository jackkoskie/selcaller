cmake_minimum_required(VERSION 3.14)

project(selcaller VERSION "1.0.0") # x-release-please-version

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(WARNING "EuroScope plugins must be built as Win32 (32-bit). Configure with: cmake -A Win32 ...")
endif()

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  if(NOT CMAKE_CXX_FLAGS MATCHES "/MP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /sdl /permissive- /DNOMINMAX")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /sdl /permissive- /DNOMINMAX")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFESTUAC:NO /ignore:4099")
  add_definitions(/D_USRDLL)
endif()

configure_file(
  ${CMAKE_SOURCE_DIR}/src/Version.h.in
  ${CMAKE_BINARY_DIR}/Version.h
)

set(ES_LIB "${CMAKE_SOURCE_DIR}/external/lib/EuroScopePlugInDll.lib")
if(NOT EXISTS "${ES_LIB}")
  message(WARNING
    "Missing ${ES_LIB}. Copy EuroScopePlugInDll.lib from %APPDATA%\\EuroScope\\PlugIn into external/lib/ before linking.")
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/external/include
)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(SOURCE_FILES
  src/main.cpp
  src/SelCallerPlugin.cpp
  src/SelCallerPlugin.h
  src/Selcal.cpp
  src/Selcal.h
  src/Version.h.in
)

add_library(selcaller SHARED ${SOURCE_FILES})

if(EXISTS "${ES_LIB}")
  target_link_libraries(selcaller PRIVATE "${ES_LIB}")
endif()

set_target_properties(selcaller PROPERTIES
  OUTPUT_NAME "selcaller"
  PREFIX ""
)
