#include "log.h"
#include "option.h"

int main(int argc, char **argv) {
	setloglevel(LOG_INFO);
	struct OPTION *op = setOPTION(argc, argv);

	freeOPTION(op);
	releaselog();
	return 0;
}
