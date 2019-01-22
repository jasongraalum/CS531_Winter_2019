IDIR=include
CC=clang
CFLAGS= -I$(IDIR) -pthread -pg -g 

llvm_out: CFLAGS=-S -I$(IDIR) -pthread -pg -g -emit-llv
mem_check: CFLAGS=-I$(IDIR) -pthread -pg -g -fsanitize=memory
thread_check: CFLAGS=-I$(IDIR) -pthread -pg -g -fsanitize=thread

ODIR=obj
LDIR=lib

LIBS= 

_DEPS = summation.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = summation.o sum_thread_1.o sum_thread_2.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

llvm_out: $(OBJ)
	$(CC) $(CFLAGS) -o summation $^ $(LIBS)

mem_check: $(OBJ)
	$(CC) $(CFLAGS) -o summation $^ $(LIBS)

thread_check: $(OBJ)
	$(CC) $(CFLAGS) -o summation $^ $(LIBS)

$(ODIR)/%.o: %.c $(DEPS)
	echo $(OBJ)
	$(CC) $(CFLAGS) -c -o $@ $< 

summation: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ summation core $(INCDIR)/*~ 
