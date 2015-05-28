#include "log.h"
#include "option.h"
#include "linefile.h"
#include "attr.h"
#include "net.h"

int main(int argc, char **argv) {
	setloglevel(LOG_INFO);
	struct OPTION *op = setOPTION(argc, argv);

	struct LineFile * lf = createLF("../DATA/movielens/movielens_3c.txt", INT, INT, INT, INT, -1);
	struct NETATTR na;
	na.direct = DIRECTED;
	NET *net = createNET(lf, na);

	printNET(net, "./xx");

	freeNET(net);
	freeLF(lf);
	freeOPTION(op);
	releaselog();
	return 0;
}
