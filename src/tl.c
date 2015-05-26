#include "log.h"

int main(int argc, char **argv) {
	setloglevel(LOG_INFO);

	releaselog();
	return 0;
}
