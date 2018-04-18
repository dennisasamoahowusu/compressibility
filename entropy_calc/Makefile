CFLAGS=-O2 -Wall -Werror -lm

default all: entropy_calc

log2_lshift16.o: log2_lshift16.c
	$(CC) $(CFLAGS) -c $? -o $@

entropy_calc.o: entropy_calc.c
	$(CC) $(CFLAGS) -c $? -o $@

avg_mean.o: avg_mean.c
	$(CC) $(CFLAGS) -c $? -o $@

shannon_f.o: shannon_f.c
	$(CC) $(CFLAGS) -c $? -o $@

heuristic.o: heuristic.c
	$(CC) $(CFLAGS) -c $? -o $@

sort.o: sort.c
	$(CC) $(CFLAGS) -c $? -o $@



entropy_calc: entropy_calc.o avg_mean.o shannon_f.o log2_lshift16.o heuristic.o sort.o
	$(CC) $(CFLAGS) -o $@ $? -lm


