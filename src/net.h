#ifndef CN_IIDNET_H
#define CN_IIDNET_H

#include "core.h"
#include "attr.h"
#include "utils.h"
#include "linefile.h"

struct NET{
	int size;
	struct CORE **core;
	struct NETATTR attr;
}; 

typedef struct NET NET;

void freeNET(NET *net);
NET *createNET(const struct LineFile * const file, struct NETATTR na);

#endif
