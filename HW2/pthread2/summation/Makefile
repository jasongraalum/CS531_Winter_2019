IDIR=include
CC=clang
DEFINES=
CFLAGS= -I$(IDIR) -I/usr/local/include -pthread 

ODIR=obj
LDIR=lib

LIBS= 

_DEPS = summation.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = summation.o single_thread.o multi_thread_global_var.o multi_thread_local_var.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

summation: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lgsl -lgslcblas $(LIBS)

summation-pg: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -pg -lgsl -lgslcblas $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ summation core $(INCDIR)/*~ 
