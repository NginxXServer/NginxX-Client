CC = gcc
CFLAGS = -Wall -g

bin = client
src = main.c http/request.c

all: $(bin)

$(bin): $(src)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(bin)
