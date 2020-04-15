main: prog2.cpp
	g++ -g prog2.cpp -o BestFirstSearch
clean:
	rm BestFirstSearch
run:
	./BestFirstSearch input.txt output.txt
