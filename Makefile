all:
	cd ./src/simulator; make
	cd ./src/tpg/tpg; make
	cd ./src/tpg/tpg_TPI_gate; make

clean:
	cd ./src/simulator; make clean
	cd ./src/tpg/tpg; make clean
	cd ./src/tpg/tpg_TPI_gate; make clean
	rm -f *_lfsr_pi.dat *_tgl_gt_tpi.dat *_tgl_gt_input.dat ff_station_*.dat

clean-all: clean
	rm -rf FLT_LIST/* OUTPUTS/*