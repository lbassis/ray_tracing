all:
	g++ -pthread -fopenmp main.cpp -o ray

prof:
	g++ -pthread -fopenmp -pg main.cpp -o ray

clean:
	rm ./ray
