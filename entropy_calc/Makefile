CFLAGS=-O2 -Wall -Werror -lm

default all: entropy_calc

entropy_calc.o: entropy_calc.c
	$(CC) $(CFLAGS) -c $? -o $@

avg_mean.o: avg_mean.c
	$(CC) $(CFLAGS) -c $? -o $@

shannon_f.o: shannon_f.c
	$(CC) $(CFLAGS) -c $? -o $@

entropy_calc: entropy_calc.o avg_mean.o shannon_f.o
	$(CC) $(CFLAGS) -o $@ $? -lm


