.PHONY: all clean

# Define required raylib variables
PROJECT_NAME       ?= game
RAYLIB_VERSION     ?= 5.0.0
RAYLIB_PATH        ?= ../..

# Define compiler path on Windows
COMPILER_PATH      ?= C:/raylib/w64devkit/bin

# Define default options
PLATFORM           ?= PLATFORM_DESKTOP

DESTDIR            ?= /usr/local
RAYLIB_INSTALL_PATH?= $(DESTDIR)/lib
RAYLIB_H_INSTALL_PATH?= $(DESTDIR)/include
RAYLIB_LIBTYPE       ?= STATIC
BUILD_MODE         ?= RELEASE
USE_EXTERNAL_GLFW  ?= FALSE
USE_WAYLAND_DISPLAY? = FALSE

# Determine PLATFORM_OS in case PLATFORM_DESKTOP selected
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(OS),Windows_NT)
        PLATFORM_OS=WINDOWS
        export PATH := $(COMPILER_PATH):$(PATH)
    else
        UNAMEOS=$(shell uname)
        ifeq ($(UNAMEOS),Linux)
            PLATFORM_OS=LINUX
        endif
        ifeq ($(UNAMEOS),FreeBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),OpenBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),NetBSD)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),DragonFly)
            PLATFORM_OS=BSD
        endif
        ifeq ($(UNAMEOS),Darwin)
            PLATFORM_OS=OSX
        endif
    endif
endif

ifeq ($(PLATFORM),PLATFORM_RPI)
    UNAMEOS=$(shell uname)
    ifeq ($(UNAMEOS),Linux)
        PLATFORM_OS=LINUX
    endif
endif

# RAYLIB_PATH adjustment for different platforms.
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        RAYLIB_PREFIX ?= ..
        RAYLIB_PATH    = $(realpath $(RAYLIB_PREFIX))
    endif
endif

# Default path for raylib on Raspberry Pi
ifeq ($(PLATFORM),PLATFORM_RPI)
    RAYLIB_PATH       ?= /home/pi/raylib
endif

# Define raylib release directory for compiled library.
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src

EXAMPLE_RUNTIME_PATH   ?= $(RAYLIB_RELEASE_PATH)

# Default C compiler: g++
CC = g++

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),OSX)
        CC = clang++
    endif
    ifeq ($(PLATFORM_OS),BSD)
        CC = clang
    endif
endif

ifeq ($(PLATFORM),PLATFORM_RPI)
    ifeq ($(USE_RPI_CROSS_COMPILER),TRUE)
        CC = $(RPI_TOOLCHAIN)/bin/arm-linux-gnueabihf-gcc
    endif
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    CC = emcc
endif

# Define default make program: Mingw32-make
MAKE = mingw32-make

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        MAKE = make
    endif
    ifeq ($(PLATFORM_OS),OSX)
        MAKE = make
    endif
endif

# Define compiler flags:
CFLAGS += -Wall -std=c++17 -D_DEFAULT_SOURCE -Wno-missing-braces

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -s -O1
endif

# Additional flags for compiler (if desired)
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        CFLAGS += $(RAYLIB_PATH)/src/raylib.rc.data
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            CFLAGS += -Wl,-rpath,$(EXAMPLE_RUNTIME_PATH)
        endif
    endif
endif

ifeq ($(PLATFORM),PLATFORM_RPI)
    CFLAGS += -std=gnu99
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    CFLAGS += -Os -s USE_GLFW=3 -s TOTAL_MEMORY=16777216 --preload-file resources
    ifeq ($(BUILD_MODE), DEBUG)
        CFLAGS += -s ASSERTIONS=1 --profiling
    endif

    CFLAGS += --shell-file $(RAYLIB_PATH)/src/shell.html
    EXT = .html
endif

# Define include paths for required headers
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external
ifneq ($(wildcard /opt/homebrew/include/.*),)
    INCLUDE_PATHS += -I/opt/homebrew/include
endif

ifeq ($(PLATFORM),PLATFORM_RPI)
    INCLUDE_PATHS += -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),BSD)
        INCLUDE_PATHS += -I/usr/local/include
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        INCLUDE_PATHS = -I$(RAYLIB_H_INSTALL_PATH) -isystem. -isystem$(RAYLIB_PATH)/src -isystem$(RAYLIB_PATH)/release/include -isystem$(RAYLIB_PATH)/src/external
    endif
endif

# Define library paths containing required libs.
LDFLAGS = -L.

ifneq ($(wildcard $(RAYLIB_RELEASE_PATH)/.*),)
    LDFLAGS += -L$(RAYLIB_RELEASE_PATH)
endif
ifneq ($(wildcard $(RAYLIB_PATH)/src/.*),)
    LDFLAGS += -L$(RAYLIB_PATH)/src
endif
ifneq ($(wildcard /opt/homebrew/lib/.*),)
    LDFLAGS += -L/opt/homebrew/lib
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),BSD)
        LDFLAGS += -L. -Lsrc -L/usr/local/lib
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        LDFLAGS = -L. -L$(RAYLIB_INSTALL_PATH) -L$(RAYLIB_RELEASE_PATH)
    endif
endif

ifeq ($(PLATFORM),PLATFORM_RPI)
    LDFLAGS += -L/opt/vc/lib
endif

# Define any libraries required on linking
ifneq ($(USE_EXTERNAL_GLFW),TRUE)
    LDLIBS = -lraylib
else
    LDLIBS = -lglfw -lraylib
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
        LDLIBS += -lopengl32 -lgdi32 -lwinmm
    endif
    ifeq ($(PLATFORM_OS),LINUX)
        LDLIBS += -lGL -lm -lpthread -ldl -lrt -lX11
        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            LDLIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
        endif
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDLIBS += -lc
        endif
    endif
    ifeq ($(PLATFORM_OS),OSX)
        LDLIBS += -framework OpenGL -framework OpenAL -framework Cocoa -framework IOKit
    endif
    ifeq ($(PLATFORM_OS),BSD)
        LDLIBS += -lGL -lpthread -lm -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor
    endif
endif

ifeq ($(PLATFORM),PLATFORM_RPI)
    LDLIBS = -lbrcmGLESv2 -lbrcmEGL -lpthread -lrt -lm -lbcm_host -ldl
endif

ifeq ($(PLATFORM),PLATFORM_WEB)
    LDLIBS = $(RAYLIB_RELEASE_PATH)/libraylib.bc
endif

# Define a recursive wildcard function to find all .cpp files in src and its subdirectories
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Define build directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Define all source files required (all .cpp files in src and its subdirectories)
SRCS = $(call rwildcard, $(SRC_DIR), *.cpp)

# Generate the list of object files based on source files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# For Android platform we call a custom Makefile.Android
ifeq ($(PLATFORM),PLATFORM_ANDROID)
    MAKEFILE_PARAMS = -f Makefile.Android 
    export PROJECT_NAME
    export SRC_DIR
else
    MAKEFILE_PARAMS = $(PROJECT_NAME)
endif

# Default target entry
all:
	$(MAKE) $(MAKEFILE_PARAMS)

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Clean everything
clean:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
		del /s /f $(OBJ_DIR)\*.o $(BUILD_DIR)\$(PROJECT_NAME).exe
    endif
    ifeq ($(PLATFORM_OS),LINUX)
		find $(OBJ_DIR) -type f -executable -delete
		rm -fv $(OBJ_DIR)/%.o
		find $(BUILD_DIR) -type f -executable -delete
    endif
    ifeq ($(PLATFORM_OS),OSX)
		find $(OBJ_DIR) -type f -perm +ugo+x -delete
		rm -f $(OBJ_DIR)/*.o
		find $(BUILD_DIR) -type f -executable -delete
    endif
endif
ifeq ($(PLATFORM),PLATFORM_RPI)
		find $(OBJ_DIR) -type f -executable -delete
		rm -fv $(OBJ_DIR)/*.o
		find $(BUILD_DIR) -type f -executable -delete
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
		del /s /f $(OBJ_DIR)\*.o $(BUILD_DIR)\$(PROJECT_NAME).html $(BUILD_DIR)\*.js
endif
	@echo Cleaning done
