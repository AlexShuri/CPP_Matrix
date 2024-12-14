CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c++17
OS = $(shell uname)

ifeq ($(OS), Linux)
	CHECKFLAGS=-lgtest -lgtest_main -lrt -lm -lstdc++ -pthread -fprofile-arcs -ftest-coverage
else
	CHECKFLAGS=-lgtest -lgtest_main -lm -lpthread -fprofile-arcs -ftest-coverage -lstdc++
endif


all: clean s21_matrix_oop.a test
	@mkdir spare_files

s21_matrix_oop.a:
	$(CC) $(CFLAGS) -c s21_matrix_oop.cc -o s21_matrix_oop.o
	ar -crs s21_matrix_oop.a s21_matrix_oop.o 
	ranlib s21_matrix_oop.a

test: clean s21_matrix_oop.a 
	$(CC) $(CHECKFLAGS) -std=c++17 --coverage s21_matrix_oop.a s21_matrix_oop_test.cc -o unit_test
	./unit_test

leaks: clean test
	leaks -atExit -- ./unit_test

check_style:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n *.cc *.h
	rm .clang-format

clean: 
	-rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt *.gch unit_test
	-rm -rf tests
	-rm -rf report
	-rm -rf spare_files
	-rm -rf .clang-format
	find . -type d -name 'tests.dSYM' -exec rm -r {} +

rebuild: clean all
