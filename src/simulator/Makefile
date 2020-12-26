CC = gcc
CFLAGS = -O3 

DEFFILE = declare.h def_flt.h def_gtype.h 
OBJ = flist.o main.o nsort.o fsim.o inject.o lfsr.o option_set.o sim_lib.o flt_simtor.o 

sim : $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ) -lm

clean:
	rm -f *.o *~ ./sim
