CXX = g++
CXXFLAGS = -c -Wall 
LD = g++
LDFLAGS = -lwiringPi

# Include local configuration to override defaults. Put all your changes
# into this file. This avoids cluttering the repository with your local
# settings.
-include Makefile.local

MODULES   := . menus menus/info menus/settings menus/system tools
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))


SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CXX) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs build/raspagercontrol

build/raspagercontrol: $(OBJ)
	$(LD) $^ -o $@ $(LDFLAGS)


checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))

#SRC = $(wildcard src/*.cpp)
#OBJ = $(SRC:src/%.cpp=build/%.o)
#BIN = build/raspagercontrol

#.PHONY: all clean

#all: $(BIN)

#$(BIN): $(OBJ)
#	$(LD) -o $@ $^ $(LDFLAGS)

#build/%.o: src/%.cpp | build
#	$(CXX) $(CXXFLAGS) -o $@ $<

#build:
#	mkdir -p $@

#clean:
#	rm -rf build/
