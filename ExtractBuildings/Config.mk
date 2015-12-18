CXX ?= g++
CC ?= gcc

CXXFLAGS += -pthread -fdiagnostics-color=auto -fmax-errors=1 -std=c++14 -O2 -Wall -Wextra -pedantic -Wuninitialized -Wstrict-overflow=3 -ffunction-sections -fdata-sections
CXXFLAGS += -I./ThirdParty/libosmium/include -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_FORTIFY_SOURCE=2

LDFLAGS += -Wl,-O1 -Wl,--hash-style=gnu -Wl,--sort-common -Wl,--build-id -Wl,--gc-sections
LDLIBS += -lstdc++ -lm -lpthread -lz
