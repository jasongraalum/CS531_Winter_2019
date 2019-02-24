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

debug: CFLAGS= -g -I$(IDIR)
debug: SUFFIX=_debug
debug: clean_debug
debug: $(PRG) 

perf: CFLAGS= -O2 -fprofile-instr-generate $(CFLAGS)
perf: SUFFIX=_perf
perf: clean_perf
perf: $(PRG) 

_OBJ = $(PRG).o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o$(SUFFIX): $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@$(SUFFIX) $< $(CFLAGS) $(DEFINES)

$(PRG): $(OBJ)
	$(CC) -o $(BDIR)/$@ $(OBJ) $(CFLAGS) $(LIBS) $(DEFINES)


clean: clean_perf
clean:
	rm -f $(ODIR)/*.o *~ $(BDIR)/sudoSolv core $(INCDIR)/*~ 

clean_debug:
	rm -f $(ODIR)/*.o_debug *~ $(BDIR)/sudoSolv_debug core $(INCDIR)/*~ 
clean_perf:
	rm -f $(ODIR)/*.o_perf *~ $(BDIR)/sudoSolv_perf core $(INCDIR)/*~ 
