IDIR=include
CC=clang
CFLAGS= -I$(IDIR) -I/usr/local/include -lgsl -lgslcblas -pthread -pg -g 

ODIR=obj
LDIR=lib

LIBS= 

_DEPS = summation.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = summation.o pth_1_gvar_cont_sum.o pth_1_gvar_cont_sum.o  pth_nth_gvar_sum.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/summation.o: summation.c $(DEPS)
	$(CC) $(DEF) $(CFLAGS) -c -o $@ $< 

$(ODIR)/pth_1_gvar_end_sum.o: pth_1_gvar_end_sum.c $(DEPS)
	$(CC) $(DEF) $(CFLAGS) -c -o $@ $< 

$(ODIR)/pth_1_gvar_cont_sum.o: pth_1_gvar_cont_sum.c $(DEPS)
	$(CC) $(DEF) $(CFLAGS) -c -o $@ $< 

$(ODIR)/pth_nth_gvar_sum.o: pth_nth_gvar_sum.c $(DEPS)
	$(CC) $(DEF) $(CFLAGS) -c -o $@ $< 

summation: $(OBJ) $(SUM_OBJ)
	$(CC) $(DEF) $(CFLAGS) -o $@ $^ $(LIBS)

summation-sp: DEF=-DSP_SUM
summation-sp: summation   
	echo $(DEF)

summation-gvar1: DEF=-DPTH_1_GVAR_CONT SUM_OBJ=$(ODIR)/pth_1_gvar_cont_sum.o
summation-gvar1: summation $(ODIR)/pth_1_gvar_cont_sum.o
	echo $(DEF)

summation-gvar2: DEF=-DPTH_1_GVAR_END
summation-gvar2: summation pth_1_gvar_end_sum
	echo $(DEF)

summation-n-gvar: DEF=-DPTH_NTH_GVAR
summation-n-gvar: summation pth_nth_gvar_sum
	echo $(DEF)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ summation core $(INCDIR)/*~ 
