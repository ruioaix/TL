#include "spath.h"
#include "utils.h"
#include "log.h"

/******************1A&average shortest path for undirect unweight.***************************************/
static void core_spath_1A_undirect_unweight_Net(int *sp, int **left, int **right, int *lNum, int *rNum, NET *net, int *STEP_END) {
	int i, j;
	int STEP = 0;
	while (*lNum && STEP != *STEP_END) {
		++STEP;
		*rNum = 0;

		for (i=0; i<*lNum; ++i) {
			int id = (*left)[i];
			for (j=0; j<net->degree[id]; ++j) {
				int neigh = net->rela[id][j];
				if (sp[neigh] == 0) {
					sp[neigh] = STEP;
					(*right)[(*rNum)++] = neigh;
				}
			}
		}
		int *tmp = *left;
		*left = *right;
		*right = tmp;
		*lNum = *rNum;
	}
}

//in net->core[0], connect information is stored.
//
//the returned sp's length is net->maxId + 1.
//sp[x] = 0 means originId is not connected to x.
//only sp[originId] = -1;
//for the other vertices, sp[x] is the shortest path between x and originId.
int *Unweighted_1toA_SPATH(int tid, NETS *net) {
	if (tid < net->core[0]->minId || tid > net->core[0]->maxId) {
		LOG(LOG_FATAL, "target id is not a valid node id");
	}
	int *sp = scalloc(net->core[0]->maxId + 1, sizeof(int));
	int *left = smalloc((net->core[0]->maxId + 1) * sizeof(int));
	int *right = smalloc((net->core[0]->maxId + 1) * sizeof(int));
	int lNum, rNum;
	lNum = 1;
	left[0] = tid;
	sp[tid] = -1;
	int STEP_END = -1;
	core_spath_1A_undirect_unweight_Net(sp, &left, &right, &lNum, &rNum, net->core[0], &STEP_END);
	free(left);
	free(right);
	return sp;
}
