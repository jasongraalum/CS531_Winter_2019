OBJ    = mergeSortMPI
CC     = mpicc
CFLAGS = -Wall -ansi -pedantic -std=c99 
LFLAGS = -lm -o $(OBJ)
COUNT = 1000000
DEF =

$(OBJ): $(OBJ).c
	$(CC) $(CFLAGS) $(OBJ).c $(LFLAGS) -D$(DEF)

run: $(OBJ)
	mpirun --map-by ppr:2:node -n 8  -hostfile mpi_hosts ./mergeSortMPI $(COUNT)
clean:
	rm -f $(OBJ)
