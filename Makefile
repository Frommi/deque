test:
	g++ -std=c++11 -Wall -Wextra -pedantic -g -pthread test.cpp -lgtest_main -lgtest -lpthread -o test.out

clean:
	rm test.out
