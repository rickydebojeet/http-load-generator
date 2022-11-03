CC = g++ -Wno-write-strings -lpthread -g --no-warnings
CPP_FILE = load_generator.cpp
HTTP_CLIENT_FILE = http_client.cpp

all: generator

generator: $(CPP_FILE) $(HTTP_CLIENT_FILE)
	$(CC) $(CPP_FILE) $(HTTP_CLIENT_FILE) -o generator

clean:
	rm -f generator
	rm -r results