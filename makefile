CXX = g++
CFLAGS = -Wall
TARGET = HYRES
SRCS_DIR = ./src

SRCS += $(SRCS_DIR)/component/hyres_CSVio.cpp
SRCS += $(SRCS_DIR)/component/hyres_solver.cpp
SRCS += $(SRCS_DIR)/HYRES.cpp

OBJ_DIR = ./obj
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

./inout/$(TARGET): $(SRCS)
	$(CXX) $(CFLAGS) $^ -o $@

$(OBJS): $(SRCS)
	$(CXX) $(CFLAGS) -c $(SRCS)