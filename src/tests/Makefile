CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra
LIBS=-lcheck -lm -lsubunit

BUILD_DIR=./build
SRC_CHECK=$(wildcard *.check)
SRC_C=$(SRC_CHECK:.check=.c)
OBJ=$(addprefix $(BUILD_DIR)/,$(SRC_C:%.c=%.o))

TARGET=s21_test_decimal.exe

S21_LIB=../s21_decimal.a

ifeq ($(wildcard $(BUILD_DIR)), )
  $(shell mkdir $(BUILD_DIR))
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(S21_LIB) -o $(BUILD_DIR)/$@ $(LIBS)

$(OBJ):
	checkmk $(SRC_CHECK) > $(SRC_C)
	$(CC) $(CFLAGS) -c $(SRC_C) -o $(OBJ)

test: $(TARGET)
	./$(BUILD_DIR)/$(TARGET)

gcov: gcov_flags $(TARGET)

gcov_flags:
	$(eval CFLAGS += -g3 --coverage)

clang_check:
	clang-format -style=Google -n *.c *.h

clang_format:
	clang-format -style=Google -i *.c

clean_obj:
	rm -rf *.c
	cd $(BUILD_DIR) && rm -rf *.o *.a *.exe *.so *.c

clean_gcov:
	cd $(BUILD_DIR) && rm -rf *.gcno *.gcda

clean: clean_obj clean_gcov
