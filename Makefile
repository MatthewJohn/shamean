
all: main.cpp
	g++ -Wall -Wextra -g main.cpp shamean.cpp -o shamean -lcrypto -static

test: test_shamean.cpp
	g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -g -I./ -c shamean.cpp -o shamean.o  -static
	g++ -fprofile-arcs -ftest-coverage -fPIC -O0 -g -I./ -o test_shamean test_shamean.cpp shamean.o -lcppunit -lcrypto
	./test_shamean
	gcovr --xml -r . > coverage.xml

clean:
	$(RM) shamean shamean.o shamean.gcda shamean.gcno test_shamean test_shamean.gcda test_shamean.gcno coverage.xml 
