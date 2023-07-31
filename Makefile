all:
	g++ -pg -fopenmp main.cpp -o ray

clean:
	rm ./ray
