all:
	g++ -pg main.cpp -o ray

prof:
	g++ main.cpp -o ray

clean:
	rm ./ray
