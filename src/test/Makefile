CC = gcc
CFLAGS = -Wall -g

bin = run_siege
src = main.c

all: $(bin)

$(bin): $(src)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(bin)
