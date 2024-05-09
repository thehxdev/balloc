CC ?= cc
CFLAGS := -Wall -Wextra -Wno-unused-result -Og -ggdb
LDFLAGS :=

DEPS := test/test.c src/balloc.c
BIN := a.out

$(BIN): $(DEPS)
	$(CC) $(CFLAGS) $(DEPS)

clean:
	rm -rf $(BIN)
