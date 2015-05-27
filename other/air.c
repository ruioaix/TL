#include "air.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>

static void tdpotn_get_all_degree(int *sp, int N, int **alld, int *alldNum, double **p_alld, double rate_airedgeChoose) {
	int *ddis = scalloc(N, sizeof(int));

	int i;
	for (i=0; i<N; ++i) {
		if (sp[i] > 0) {
			ddis[sp[i]]++;
		}
	}
	*alld = smalloc(N*sizeof(int));
	*alldNum = 0;
	for (i=2; i<N; ++i) {
		if (ddis[i]) {
			(*alld)[(*alldNum)++] = i;
		}
	}
	free(ddis);

	*p_alld = malloc((*alldNum)*sizeof(double));
	for (i=0; i<*alldNum; ++i) {
		(*p_alld)[i] = pow((*alld)[i], 0-rate_airedgeChoose);
	}
	double total = 0;
	for (i=0; i<*alldNum; ++i) {
		total += (*p_alld)[i];
	}
	for (i=0; i<*alldNum; ++i) {
		(*p_alld)[i] /= total;
	}
	for (i=1; i<*alldNum; ++i) {
		(*p_alld)[i] += (*p_alld)[i-1];
	}
}

struct LineFile *air(NET *net, int factor_constrain, double rate_airedgeChoose, double rate_airedgeCost, double rate_airedgeActualLength) {
	int *sp = spath_1A_undirect_unweight_Net(net, 0);

	int *alld, alldNum; 
	double *p_alld;
	tdpotn_get_all_degree(sp, net->maxId + 1, &alld, &alldNum, &p_alld, rate_airedgeChoose);
	free(sp);

	int N = net->maxId + 1;
	double limit = (double)N*factor_constrain;

	int *id1 = malloc(factor_constrain*N*sizeof(int));
	int *id2 = malloc(factor_constrain*N*sizeof(int));
	double *weight = malloc(factor_constrain*N*sizeof(double));
	int *hash1 = calloc((net->maxId + 1)*3, sizeof(int));
	int *hash2 = calloc((net->maxId + 1)*2, sizeof(int));
	int *hash3 = calloc((net->maxId + 1)*3, sizeof(int));
	int idNum = 0;
	int badluck = 0;
	double totalL = 0;
	while (1) {
		double chooseSPL = get_d_MTPR();
		int splength = 0;
		int i;
		for (i=0; i<alldNum; ++i) {
			if (p_alld[i] > chooseSPL) {
				splength = alld[i];
				break;
			}
		}
		double dsplength = pow(splength, theta);
		double tmp = totalL + dsplength;
		if (tmp > limit && idNum > 0) {
			break;
		}
		else if (tmp > limit) {
			continue;
		}
		int i1 = get_i31_MTPR()%(net->maxId + 1);
		int lNum;
		int *left=NULL;
		spath_1A_step_undirect_unweight_Net(net, i1, splength, &lNum, &left);
		if (lNum > 0) {
			int random = get_i31_MTPR()%lNum;
			int i2 = left[random];
			int min = i1<i2?i1:i2;
			int max = i1>i2?i1:i2;
			if (hash1[min + 2*max] && hash2[min + max] && hash3[min*2 + max]) {
				//printlp("not lucky, drop on same positon. try again.\n");
				badluck ++;
				free(left);
				continue;
			}
			hash1[min + 2*max] = 1;
			hash2[min + max] = 1;
			hash3[min*2 + max] = 1;
			id1[idNum] = i1;
			id2[idNum] = i2;
			weight[idNum] = pow(splength, lambda);
			++idNum;
			totalL += dsplength;
		}
		free(left);
	}
	free(hash1);
	free(hash2);
	free(hash3);
	free(p_alld);
	free(alld);
	printgf("badluck: %d, NumofAddedLinks: %d", badluck, idNum);

	struct LineFile *lf = create_LineFile(NULL);
	lf->i1 = id1;
	lf->i2 = id2;
	lf->d1 = weight;
	lf->linesNum = idNum;
	lf->memNum = factor_constrain*N*sizeof(int);
	lf->filename = "air";

	return lf;
}
