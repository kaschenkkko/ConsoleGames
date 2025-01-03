CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Games
PINGPONG = ping_pong

all: clang $(PINGPONG)

clean:
	rm -f $(PINGPONG)
	rm -f *.o

rebuild: clean all

clang:
	clang-format -n games/*.c
	cppcheck --enable=all --suppress=missingIncludeSystem games/*.c

# PING-PONG
$(PINGPONG): ping_pong.o
	$(CC) $(CFLAGS) $^ -o $@

ping_pong.o: games/ping_pong.c
	$(CC) $(CFLAGS) -c $^ -o $@
