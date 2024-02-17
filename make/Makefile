
PROJ_DIR = $(shell dirname $(shell pwd))
SRC_DIR = $(PROJ_DIR)/src
INCLUDE_DIR = $(PROJ_DIR)/include
BIN_DIR = $(PROJ_DIR)/bin
OBJ_DIR = $(BIN_DIR)/obj
LIB_DIR = $(BIN_DIR)/lib
TEST_DIR = $(PROJ_DIR)/test

LIB_NAME_MINUS_LIB = ml
LIB_NAME = lib$(LIB_NAME_MINUS_LIB)



CC = gcc
CFLAGS_INCLUDE = -I$(INCLUDE_DIR)
CFLAGS_LINK = -L$(LIB_DIR) -l$(LIB_NAME_MINUS_LIB)
CFLAGS_WERR = -ansi -pedantic-errors -Wall -fstack-protector-all -Werror

CCFLAGS_SO = $(CC) -shared
CCFLAGS_ARCH = ar -rcs
CCFLAGS_LIB_OBJS = $(CC) $(CFLAGS_INCLUDE) $(CFLAGS_WERR) -c

CCFLAGS_TESTS = $(CC) $(CFLAGS_INCLUDE) $(CFLAGS_LINK) $(CFLAGS_WERR)



TEST_C_FILES = $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_EXECS = $(TEST_C_FILES:.c=_executable)
TEST_EXECS_DSYM = $(addsuffix .dSYM, $(TEST_EXECS))
TEST_EXECS_OTHERS = $(TEST_EXECS_DSYM)

LIB_FILENAMES = libml knn data
LIB_OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(LIB_FILENAMES)))

LIB_SO = $(LIB_DIR)/$(LIB_NAME).so
LIB_A = $(LIB_DIR)/$(LIB_NAME).a



all: lib test

test: $(TEST_EXECS)

$(TEST_EXECS): %_executable: %.c $(LIB_SO) $(LIB_A)
	$(CCFLAGS_TESTS) -o $@ $<

lib: $(LIB_SO) $(LIB_A)

$(LIB_SO): $(LIB_OBJS)
	$(CCFLAGS_SO) -o $(LIB_SO) $(LIB_OBJS)

$(LIB_A): $(LIB_OBJS)
	$(CCFLAGS_ARCH) $(LIB_A) $(LIB_OBJS)

$(LIB_OBJS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CCFLAGS_LIB_OBJS) -o $@ $<

clean: 
	rm -f $(LIB_SO) $(LIB_OBJS) $(LIB_A) $(TEST_EXECS)
	rm -rf $(TEST_EXECS_OTHERS)

.PHONY: all lib test clean
