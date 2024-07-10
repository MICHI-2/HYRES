CXX = g++
CFLAGS = -Wall
TARGET = HYRES
SRCS_DIR = ./src

SRCS += $(SRCS_DIR)/component/hyres_CSVio.cpp
SRCS += $(SRCS_DIR)/component/hyres_solver.cpp
SRCS += $(SRCS_DIR)/HYRES.cpp

OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

DEBUG_FLAGS = -g -o0
RELEASE_FLAGS = -O3

all: debug release

./inout/$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRCS_DIR)/component/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: ./inout/$(TARGET)_debug

./inout/$(TARGET)_debug: $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

release: CFLAGS += $(RELEASE_FLAGS)
release: ./inout/$(TARGET)_release

./inout/$(TARGET)_release: $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

clean:
	del /s *.o
	del .\inout\HYRES_debug.exe
	del .\inout\HYRES_release.exe