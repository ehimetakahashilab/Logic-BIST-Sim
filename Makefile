all:
	cd ./src/simulator; make
	cd ./src/tpg/tpg; make
	cd ./src/tpg/tpg_TPI_gate; make

clean:
	cd ./src/simulator; make clean
	cd ./src/tpg/tpg; make clean
	cd ./src/tpg/tpg_TPI_gate; make clean

clean-all: clean
	rm -rf FLT_LIST/* OUTPUTS/*