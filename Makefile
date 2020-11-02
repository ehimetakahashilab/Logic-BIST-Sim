all:
	cd ./src/simulator; make
	cd ./src/tpg; make
	cd ./src/tpg_TPI_ff; make
	cd ./src/tpg_TPI_gate; make

clean:
	cd ./src/simulator; make clean
	cd ./src/tpg; make clean
	cd ./src/tpg_TPI_ff; make clean
	cd ./src/tpg_TPI_gate; make clean