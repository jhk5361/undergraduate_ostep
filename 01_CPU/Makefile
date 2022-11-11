SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
INC_DIR = \
		  ./src \
		  -I ./src/utility \

CXX = gcc

CXXFLAGS += \
	-g \
	-O0 \
	-Wall \
	-fsanitize=address \
#	-O2 \

LIBS += \
	-lpthread \
	-lrt \

OBJ_SRC += \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/my_sched.c \
	$(SRC_DIR)/cproc.c \
	$(SRC_DIR)/utility/list.c\
	
TARGET_OBJ =\
		$(patsubst %.c,%.o,$(OBJ_SRC))\

all: my_cpu

my_cpu: $(TARGET_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $(BIN_DIR)/$@ $^ $(CXXFLAGS) $(LIBS) -I$(INC_DIR)

.c.o:
	$(CXX) $(CXXFLAGS) $(LIBS) -c $< -o $@ -I$(INC_DIR)

-include $(OBJS:.o=.d)


clean:
	@rm -vf $(BIN_DIR)/*
	@rm -vf $(OBJ_DIR)/*.o
	@rm -vf $(SRC_DIR)/*.o

