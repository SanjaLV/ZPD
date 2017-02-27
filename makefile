Main:
	g++ main.cpp -O3 -o $@ -std=gnu++11
start:
	./Main
	
run: 	Main start

.PHONY: Main
