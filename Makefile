all:
	g++ -pthread main.cpp -o ray

prof:
	g++ -pthread -pg main.cpp -o ray

clean:
	rm ./ray
