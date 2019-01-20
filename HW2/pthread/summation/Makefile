IDIR=include
CC=gcc
CFLAGS=-I$(IDIR) -pthread

ODIR=obj
LDIR=lib

LIBS= 

_DEPS = summation.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = summation.o sum_thread.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	echo $(OBJ)
	$(CC) $(CFLAGS) -c -o $@ $< 

summation: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
