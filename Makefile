all:
	g++ main.cpp -o ray

prof:
	g++ -pg main.cpp -o ray

clean:
	rm ./ray
