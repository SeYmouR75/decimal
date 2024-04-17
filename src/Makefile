CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

TEST_DIR=tests
TEST_EXE=s21_test_decimal.exe
BUILD_DIR=build
REPORT_DIR=report

ifeq ($(wildcard $(BUILD_DIR)), )
  $(shell mkdir $(BUILD_DIR))
endif
ifeq ($(wildcard $(REPORT_DIR)), )
  $(shell mkdir $(REPORT_DIR))
endif

SRC=$(wildcard *.c)
OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:%.c=%.o))
TARGET=s21_decimal.a

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $@ $(OBJ)

$(BUILD_DIR)/%.o: %.c   
	$(CC) $(CFLAGS) -c $< -o $@ 

test: clean $(TARGET)
	cd $(TEST_DIR) && make test
#	cd $(TEST_DIR) && valgrind --tool=memcheck --leak-check=yes $(BUILD_DIR)/$(TEST_EXE)

gcov_report: clean gcov_flags $(TARGET)	
	cd $(TEST_DIR) && make gcov
	./$(TEST_DIR)/$(BUILD_DIR)/$(TEST_EXE)
	lcov -t "coverage_report" -o s21_decimal.info -c -d $(BUILD_DIR) -d $(TEST_DIR)/$(BUILD_DIR)
	genhtml s21_decimal.info -o report 
	xdg-open $(REPORT_DIR)/index.html

gcov_flags: 
	$(eval CFLAGS += -g3 --coverage)

clean_obj:
	cd $(BUILD_DIR) && rm -rf *.o *.a *.exe *.so
	rm -rf *.a 

clean_gcov:
	cd $(BUILD_DIR) && rm -rf *.gcno *.gcda
	rm -rf $(REPORT_DIR)/*
	rm -rf s21_decimal.info

clean: clean_obj clean_gcov
	cd $(TEST_DIR) && make clean

clang_check:
	clang-format -style=Google -n *.c *.h
	cd $(TEST_DIR) && make clang_check

clang_format:
	clang-format -style=Google -i *.c *.h
	cd $(TEST_DIR) && make clang_format

rebuild: clean all
