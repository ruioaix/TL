#ifndef AIR_H
#define AIR_H

#include "linefile.h"
#include "net.h"

struct LineFile *air(NET *net, int factor_constrain, double rate_airedgeChoose, double rate_airedgeCost, double rate_airedgeActualLength);

#endif
