.PHONY: clean debug perf

PRG=quickSort
IDIR=./include
BDIR=./bin
SDIR=./src
ODIR=obj
LDIR=lib

CC=clang
DEFINES=
CFLAGS= -pthread -I$(IDIR)

LIBS= -lm 

all: $(PRG)

_OBJ = $(PRG).o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

$(PRG): $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)


clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoSolv core $(INCDIR)/*~ 
