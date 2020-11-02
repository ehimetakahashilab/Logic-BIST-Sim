CC = gcc
CFLAGS = -O3 
OBJ = lfsr.o

PROG := lfsr_gt

$(PROG):$(OBJ) 
	$(CC) -o $@ $(CFLAGS) $(OBJ)

clean:
	rm -f *.o *~ lfsr_gt
	