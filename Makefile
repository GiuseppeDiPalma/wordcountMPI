wc_mpi.out: wc_mpi.o fileManage.o utils.o
	mpicc wc_mpi.o fileManage.o utils.o -lm -o wc_mpi.out

utils.o: utils.c
	gcc -c utils.c

filemanage.o: fileManage.c
	gcc -c fileManage.c

wc_mpi.o: wc_mpi.c fileManage.o utils.o
	mpicc -c wc_mpi.c

clean:
	rm *.o *.out