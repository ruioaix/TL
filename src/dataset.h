/**
 * this file is used to generate all kinds of data set.
 * the data set will be represented with struct LineFile or just print into file.
 */
#ifndef DATASET_H
#define DATASET_H

#include "linefile.h"

enum DSATTR {
	CROSSOVER, NONCROSSOVER,
	DIRECT, NONDIRECT
};
struct DSATTRS {
	enum DSATTR crossover;
	enum DSATTR direct;
};

struct LineFile * latticeDS(int L, struct DSATTRS dsas);
struct LineFile * lineDS(int N, struct DSATTRS dsas);

#endif
