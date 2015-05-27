#ifndef OPTION_H
#define OPTION_H 

#include <stdbool.h>

struct OPTION {
	char *logfilename;
	unsigned long num_randomseed;

	bool ds_line;
	int num_line_node;
	bool ds_lattice;
	int num_lattice_side;
	bool ds_crossover;
	bool ds_direct;

	int factor_constrain;

	double rate_airedgeCost;
	double rate_airedgeActualLength;
	
	bool alg_aspcoupling;
	bool alg_aspgini;
};

struct OPTION *setOPTION(int argc, char **argv);
void freeOPTION(struct OPTION *op);

#endif
