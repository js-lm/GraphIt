cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(GraphIt)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set static linking options for Release mode
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  # Static linking for MSVC
  if(MSVC)
    # Static linking of the runtime libraries for MSVC
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    # Replace dynamic runtime with static runtime
    foreach(flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
            CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif()
    endforeach()
  endif()

  # Static linking for MinGW/GCC
  if(MINGW)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -static -static-libgcc")
  endif()
endif()

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if(NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if(NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    # Configure raylib to use static runtime with MSVC
    if(MSVC AND (CMAKE_BUILD_TYPE STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
      set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
    endif()
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

# Define a recursive file search function
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Our Project
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/resources
)

# Link libraries
target_link_libraries(${PROJECT_NAME} raylib)

# Icon
if(WIN32)
  # Windows
  if(EXISTS "${CMAKE_SOURCE_DIR}/resources/icon/icon.ico")
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/resources/icon/icon.rc")
    configure_file(
      "${CMAKE_SOURCE_DIR}/resources/icon/icon.rc"
      "${APP_ICON_RESOURCE_WINDOWS}"
    )
    target_sources(${PROJECT_NAME} PRIVATE "${APP_ICON_RESOURCE_WINDOWS}")
  endif()
elseif(APPLE)
  # macOS
  set(MACOSX_BUNDLE_ICON_FILE icon.icns)
  if(EXISTS "${CMAKE_SOURCE_DIR}/resources/icon/icon.icns")
    set_target_properties(${PROJECT_NAME} PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
      RESOURCE "${CMAKE_SOURCE_DIR}/resources/icon/icon.icns"
    )
  endif()
endif()

# Web Configurations
if("${PLATFORM}" STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an HTML file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Windows-specific configurations
if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm gdi32 opengl32)
    
    # For Release builds on Windows, enable static linking of system libraries
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        # Static linking options for MSVC
        if(MSVC)
            set_target_properties(${PROJECT_NAME} PROPERTIES
                LINK_FLAGS "/INCREMENTAL:NO /NODEFAULTLIB:MSVCRT /NODEFAULTLIB:MSVCRTD"
            )
        endif()
        
        # Static linking options for MinGW
        if(MINGW)
            set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
        endif()
    endif()
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()