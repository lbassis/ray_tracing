all:
	g++ -pg -pthread main.cpp -o ray

clean:
	rm ./ray
