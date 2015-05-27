#include "option.h"
#include "log.h"
#include "random.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static void display_usage(void) {
	puts("TL - Two Level Network. (May 2015)\n");
	puts("usage: tl [OPTION]\n");
	puts("OPTION common:");
	puts("  -h:  help");
	puts("  -o logfilename:");
	puts("       File the log system will output log to");
	puts("  -s unsignedlongValue: ");
	puts("       Random seed");
	puts("");
	puts("OPTION:");
	puts("  -l:  line dataset");
	puts("  --num-line-node intValue:  ");
	puts("       the number of the nodes in line dataset");
	puts("  -L:  lattice dataset");
	puts("  --num-lattice-side intValue:  ");
	puts("       the number of the nodes in one lattice side");
	puts("       so the number of the nodes in lattice is L^2");
	puts("  -c:  net crossover, if not set, the net will be non-crossover by default");
	puts("  -d:  net directed, if not set, the net will be undirected by default");
	puts("  --factor-constrain intValue:  ");
	puts("       this number * the number of nodes in net = constrain which will be used");
	puts("  -C doubleValue:  ");
	puts("       this value is used to define the cost of an edge from constrain.");
	puts("       by default the cost of an edge is the length of the edge in ground net.");
	puts("       that means the value by default to be 1. now the cost is length^theta, [0.5-2]");
	puts("  -A doubleValue:  ");
	puts("       this value is used to define the actual length ot the new edges.");
	puts("       by default, the actual length is 1 (that means this value is default to be 0) but now it is length^value, [0-1].");
	puts("");
	exit(0);
}

static void init_OPTION(struct OPTION *op) {
	op->logfilename = NULL;
	op->num_randomseed= 1;

	op->ds_line = false;
	op->num_line_node = 100;
	op->ds_lattice = false;
	op->num_lattice_side = 50;
	op->ds_crossover = false;
	op->ds_direct = false;

	op->factor_constrain = 5;
	
	//[0.5, 2]
	op->rate_airedgeCost = 1.0;
	//[0, 1]
	op->rate_airedgeActualLength = 0;

}

void freeOPTION(struct OPTION *op) {
	free(op);
}

static void verify_OPTION(struct OPTION *op);
static void info_OPTION(struct OPTION *op);

struct OPTION *setOPTION(int argc, char **argv) {
	if (argc == 1) display_usage();

	struct OPTION *op = smalloc(sizeof(struct OPTION));
	init_OPTION(op);

	static const char *short_options = "ho:s:lLcdC:A:";
	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"log-file", required_argument, NULL, 'o'},
		{"num-randomseed", required_argument, NULL, 's'},

		{"ds-line", no_argument, NULL, 'l'},
		{"num-line-node", required_argument, NULL, 300},
		{"ds-lattice", no_argument, NULL, 'L'},
		{"num-lattice-side", required_argument, NULL, 301},
		{"ds-crossover", no_argument, NULL, 'c'},
		{"ds-direct", no_argument, NULL, 'd'},
		{"factor-constrain", required_argument, NULL, 302},

		{"rate-airedge-cost", required_argument, NULL, 'C'},
		{"rate-airedge-actuallength", required_argument, NULL, 'A'},

		{0, 0, 0, 0},
	};
	int longIndex = 0;
	int opt;
	do {
		opt = getopt_long(argc, argv, short_options, long_options, &longIndex);
		if (opt == -1) break;
		switch (opt) {
			case 'h':
				display_usage();
				break;
			case 'o':
				op->logfilename = optarg;
				break;
			case 's':
				op->num_randomseed = strtol(optarg, NULL, 10);
				break;

			case 'l':
				op->ds_line = true;
				break;
			case 300:
				op->num_line_node = strtol(optarg, NULL, 10);
				break;
			case 'L':
				op->ds_lattice = true;
				break;
			case 301:
				op->num_lattice_side = strtol(optarg, NULL, 10);
				break;
			case 'c':
				op->ds_crossover = true;
				break;
			case 'd':
				op->ds_direct = true;
				break;
			case 302:
				op->factor_constrain = strtol(optarg, NULL, 10);
				break;
			case 'C':
				op->rate_airedgeCost = strtod(optarg, NULL);
				break;
			case 'A':
				op->rate_airedgeActualLength = strtod(optarg, NULL);
				break;
			case '?':
				break;
			default:
				abort();
		}
	} while (1);

	loginit(op->logfilename, getloglevel());
	setRGseed(op->num_randomseed);

	verify_OPTION(op);
	info_OPTION(op);

	return op;
}

static void verify_OPTION(struct OPTION *op) {
	//dataset
	if (!( op->ds_line || op->ds_lattice)) {
		LOG(LOG_FATAL, "no dataset selected, what do you want to calculate?");
	}
	if (op->ds_line && op->ds_lattice) {
		LOG(LOG_FATAL, "Sorry, for now you can only choose one kind of dataset: line or lattice.");
	}

	if (op->ds_line) {
		if (op->num_line_node < 2) {
			LOG(LOG_FATAL, "the nodes number of line dataset is set to %d, are you sure?", op->num_line_node);
		}
	}
	if (op->ds_lattice) {
		if (op->num_lattice_side < 2) {
			LOG(LOG_FATAL, "the side nodes number of lattice dataset is set to %d, are you sure?", op->num_lattice_side);
		}
	}

	if (op->factor_constrain < 0) {
		LOG(LOG_FATAL, "the constrain factor is set to %d, are you sure?", op->factor_constrain);
	}

	if (op->rate_airedgeCost < 0.5 || op->rate_airedgeCost > 2) {
		LOG(LOG_FATAL, "the rate_airedgeCost is set to %f, that's not valid, set to [0.5, 2].", op->rate_airedgeCost);
	}

	if (op->rate_airedgeActualLength < 0 || op->rate_airedgeActualLength > 1) {
		LOG(LOG_FATAL, "the rate_airedgeActualLength is set to %f, that's not valid, set to [0, 1].", op->rate_airedgeActualLength);
	}

}

static void info_OPTION(struct OPTION *op) {
	if (op->ds_line) {
		LOG(LOG_INFO, "line dataset: used");
		LOG(LOG_INFO, "line ds, nodes number is %d", op->num_line_node);
	}
	if (op->ds_lattice) {
		LOG(LOG_INFO, "lattice dataset: used");
		LOG(LOG_INFO, "lattice ds, sides number is %d", op->num_lattice_side);
	}

	if (op->ds_crossover) LOG(LOG_INFO, "net will be crossover");
	else LOG(LOG_INFO, "net will be non-crossover");

	if (op->ds_direct) LOG(LOG_INFO, "net will be directed");
	else LOG(LOG_INFO, "net will be undirected");

	LOG(LOG_INFO, "The constrain factor is %d", op->factor_constrain);
	LOG(LOG_INFO, "The cost rate is %f", op->rate_airedgeCost);
	LOG(LOG_INFO, "The actual length rate is %f", op->rate_airedgeActualLength);
}
