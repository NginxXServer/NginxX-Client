CC = gcc
CFLAGS = -Wall -g

bin = client
src = main.c wrk/wrk.c

all: $(bin)

$(bin): $(src)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(bin)
