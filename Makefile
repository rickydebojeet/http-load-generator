CC = g++ -Wno-write-strings -lpthread -g --no-warnings
CPP_FILE = load_generator.cpp

all: generator

generator: $(CPP_FILE)
	$(CC) $(CPP_FILE) -o generator

clean:
	rm -f generator