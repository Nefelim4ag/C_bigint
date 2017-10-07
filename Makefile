CFLAGS = -O2 -Wall -lm

default all: bigint.out

bigint.o: bigint.c
	$(CC) $(CFLAGS) -c $? -o $@

bigint.out: bigint.o
	$(CC) $(CFLAGS) -o $@ $?

clean: ## Cleanup
	git clean -dfx

help: ## Show help
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##/\t/'
