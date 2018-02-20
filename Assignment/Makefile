all:
	gcc ignoreLine.c -o ignoreLine
	#cd ../sim_2_70/; make test
	ln -s "$(pwd)"/../sim_2_70/sim_c
clean:
	rm -vf ignoreLine
	rm -vf sim_c
	#cd ../sim_2_70/; make clean
fresh:
	rm -Rvf codebase/*
	rm -Rvf repo/*
	rm -Rvf result/*
