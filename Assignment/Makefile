#May Need to comment some lines from ../SimilarityChecker/Makefile
# Search define COMMENT in the Makefile

all:
	gcc ignoreLine.c -o ignoreLine
	cd ../SimilarityChecker/; make test
	ln -sf "$(CURDIR)"/../SimilarityChecker/sim_c
	ln -sf "$(CURDIR)"/../SimilarityChecker/sim_c++
	ln -sf "$(CURDIR)"/../SimilarityChecker/sim_text
clean:
	rm -vf ignoreLine
	rm -vf sim_c
	#cd ../sim_2_70/; make clean
fresh:
	rm -Rvf codebase/*
	rm -Rvf repo/*
	rm -Rvf result/*
