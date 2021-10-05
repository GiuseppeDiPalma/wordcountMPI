word_count_mpi.out: example.o
	mpicc example.o -lm -o word_count_mpi.out

example.o: example.c
	mpicc -c example.c

clean:
	rm *.o *.out