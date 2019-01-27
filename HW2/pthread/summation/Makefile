IDIR=include
CC=clang
DEFINES=-DTEST
#CFLAGS= -I$(IDIR) -I/usr/local/include -lgsl -lgslcblas -pthread -pg -g 
CFLAGS= -I$(IDIR) $(DEFINES) -I/usr/local/include -pthread -pg -g 

ODIR=obj
LDIR=lib

LIBS= 

_DEPS = summation.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = summation.o pth_1_gvar_cont_sum.o pth_1_gvar_cont_sum.o  pth_nth_gvar_sum.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) $(DEFINES) -c -o $@ $< $(CFLAGS)

summation-ng : DEFINES = -DPTH_NTH_GVA 
summation-ng : summation.o $(ODIR)/pth_nth_gvar.o $(DEPS)
	$(CC) $(DEFINES) $(CFLAGS) -c -o $@ $< 

summation-sp: DEFINES = -DDPTH_SP
summation-sp: summation.o $(DEPGS)
	$(CC) $(DEFINES) $(CFLAGS) -c -o $@ $< 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ summation core $(INCDIR)/*~ 
