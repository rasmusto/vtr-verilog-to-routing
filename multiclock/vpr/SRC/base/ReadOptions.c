#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "vpr_types.h"
#include "OptionTokens.h"
#include "ReadOptions.h"
#include "read_settings.h"

static boolean EchoEnabled;

/******** Function prototypes ********/

static char **ReadBaseToken(INP char **Args, OUTP enum e_OptionBaseToken *Token);
static void Error(INP const char *Token);
static char **ProcessOption(INP char **Args, INOUTP t_options * Options);
static void MergeOptions(INOUTP t_options * dest, INP t_options * src, int id);
static char **ReadFloat(INP char **Args, OUTP float *Val);
static char **ReadInt(INP char **Args, OUTP int *Val);
static char **ReadOnOff(INP char **Args, OUTP boolean * Val);
static char **ReadClusterSeed(INP char **Args, OUTP enum e_cluster_seed *Type);
static char **ReadFixPins(INP char **Args, OUTP char **PinFile);
static char **ReadPlaceAlgorithm(INP char **Args,
		OUTP enum e_place_algorithm *Algo);
static char **ReadRouterAlgorithm(INP char **Args,
		OUTP enum e_router_algorithm *Algo);
static char **ReadPackerAlgorithm(INP char **Args,
		OUTP enum e_packer_algorithm *Algo);
static char **ReadBaseCostType(INP char **Args,
		OUTP enum e_base_cost_type *BaseCostType);
static char **ReadRouteType(INP char **Args, OUTP enum e_route_type *Type);
static char **ReadString(INP char **Args, OUTP char **Val);

/******** Globally Accessible Function ********/
boolean GetEchoOption(void) {
	return EchoEnabled;
}

void SetEchoOption(boolean echo_enabled) {
	EchoEnabled = echo_enabled;
}

/******** Subroutine implementations ********/

void ReadOptions(INP int argc, INP char **argv, OUTP t_options * Options) {
	char **Args, **head;

	/* Clear values and pointers to zero */
	memset(Options, 0, sizeof(t_options));

	/* Alloc a new pointer list for args with a NULL at end.
	 * This makes parsing the same as for archfile for consistency.
	 * Skips the first arg as it is the program image path */
	--argc;
	++argv;
	head = Args = (char **) my_malloc(sizeof(char *) * (argc + 1));
	memcpy(Args, argv, (sizeof(char *) * argc));
	Args[argc] = NULL;

	/* Go through the command line args. If they have hyphens they are 
	 * options. Otherwise assume they are part of the four mandatory
	 * arguments */
	while (*Args) {
		if (strncmp("--", *Args, 2) == 0) {
			*Args += 2; /* Skip the prefix */
			Args = ProcessOption(Args, Options);
		} else if (strncmp("-", *Args, 1) == 0) {
			*Args += 1; /* Skip the prefix */
			Args = ProcessOption(Args, Options);
		} else if (NULL == Options->ArchFile) {
			Options->ArchFile = my_strdup(*Args);
			++Args;
		} else if (NULL == Options->CircuitName) {
			Options->CircuitName = my_strdup(*Args);
			/*if the user entered the circuit name with the .blif extension, remove it now*/
			if (!strcmp(
					Options->CircuitName + strlen(Options->CircuitName)
							- strlen(".blif"), ".blif")) {
				Options->CircuitName[strlen(Options->CircuitName) - 5] = '\0';
			}
			++Args;
		} else if (NULL == Options->SDCFile) {
			Options->SDCFile = my_strdup(*Args);
			++Args;
		} else {
			/* Not an option and arch and net already specified so fail */
			Error(*Args);
		}

		if (Options->Count[OT_SETTINGS_FILE] != Options->read_settings)
		{
			int tmp_argc = 0;
			char **tmp_argv = NULL;
			t_options SettingsFileOptions;

			tmp_argc = read_settings_file(Options->SettingsFile, &tmp_argv);

			ReadOptions(tmp_argc, tmp_argv, &SettingsFileOptions);

			MergeOptions(Options, &SettingsFileOptions, Options->Count[OT_SETTINGS_FILE]);

			Options->read_settings = Options->Count[OT_SETTINGS_FILE];

			/* clean up local data structures */
			free(tmp_argv);
		}
	}
	free(head);
}

static char **
ProcessOption(INP char **Args, INOUTP t_options * Options) {
	enum e_OptionBaseToken Token;

	Args = ReadBaseToken(Args, &Token);

	if (Token < OT_BASE_UNKNOWN) {
		/* If this was previously set by a lower priority source
		 * (ie. a settings file), reset the provenance and the
		 * count */
		if (Options->Provenance[Token])
		{
			Options->Provenance[Token] = 0;
			Options->Count[Token] = 1;
		}
		else
			++Options->Count[Token];
	}

	switch (Token) {
		/* File naming options */
	case OT_BLIF_FILE:
		return ReadString(Args, &Options->BlifFile);
	case OT_NET_FILE:
		return ReadString(Args, &Options->NetFile);
	case OT_PLACE_FILE:
		return ReadString(Args, &Options->PlaceFile);
	case OT_ROUTE_FILE:
		return ReadString(Args, &Options->RouteFile);
	case OT_SETTINGS_FILE:
		return ReadString(Args, &Options->SettingsFile);
		/* General Options */
	case OT_NODISP:
		return Args;
	case OT_AUTO:
		return ReadInt(Args, &Options->GraphPause);
	case OT_PACK:
	case OT_ROUTE:
	case OT_PLACE:
		return Args;
	case OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY:
		return ReadFloat(Args, &Options->constant_net_delay);
	case OT_FAST:
	case OT_FULL_STATS:
		return Args;
	case OT_TIMING_ANALYSIS:
		return ReadOnOff(Args, &Options->TimingAnalysis);
	case OT_OUTFILE_PREFIX:
		return ReadString(Args, &Options->out_file_prefix);
	case OT_CREATE_ECHO_FILE:
		return ReadOnOff(Args, &Options->CreateEchoFile);

		/* Clustering Options */
	case OT_GLOBAL_CLOCKS:
		return ReadOnOff(Args, &Options->global_clocks);
	case OT_HILL_CLIMBING_FLAG:
		return ReadOnOff(Args, &Options->hill_climbing_flag);
	case OT_SWEEP_HANGING_NETS_AND_INPUTS:
		return ReadOnOff(Args, &Options->sweep_hanging_nets_and_inputs);
	case OT_TIMING_DRIVEN_CLUSTERING:
		return ReadOnOff(Args, &Options->timing_driven);
	case OT_CLUSTER_SEED:
		return ReadClusterSeed(Args, &Options->cluster_seed_type);
	case OT_ALPHA_CLUSTERING:
		return ReadFloat(Args, &Options->alpha);
	case OT_BETA_CLUSTERING:
		return ReadFloat(Args, &Options->beta);
	case OT_RECOMPUTE_TIMING_AFTER:
		return ReadInt(Args, &Options->recompute_timing_after);
	case OT_CLUSTER_BLOCK_DELAY:
		return ReadFloat(Args, &Options->block_delay);
	case OT_ALLOW_UNRELATED_CLUSTERING:
		return ReadOnOff(Args, &Options->allow_unrelated_clustering);
	case OT_ALLOW_EARLY_EXIT:
		return ReadOnOff(Args, &Options->allow_early_exit);
	case OT_INTRA_CLUSTER_NET_DELAY:
		return ReadFloat(Args, &Options->intra_cluster_net_delay);
	case OT_INTER_CLUSTER_NET_DELAY:
		return ReadFloat(Args, &Options->inter_cluster_net_delay);
	case OT_CONNECTION_DRIVEN_CLUSTERING:
		return ReadOnOff(Args, &Options->connection_driven);
	case OT_SKIP_CLUSTERING:
		return Args;
	case OT_PACKER_ALGORITHM:
		return ReadPackerAlgorithm(Args, &Options->packer_algorithm);

		/* Placer Options */
	case OT_PLACE_ALGORITHM:
		return ReadPlaceAlgorithm(Args, &Options->PlaceAlgorithm);
	case OT_INIT_T:
		return ReadFloat(Args, &Options->PlaceInitT);
	case OT_EXIT_T:
		return ReadFloat(Args, &Options->PlaceExitT);
	case OT_ALPHA_T:
		return ReadFloat(Args, &Options->PlaceAlphaT);
	case OT_INNER_NUM:
		return ReadFloat(Args, &Options->PlaceInnerNum);
	case OT_SEED:
		return ReadInt(Args, &Options->Seed);
	case OT_PLACE_COST_EXP:
		return ReadFloat(Args, &Options->place_cost_exp);
	case OT_PLACE_CHAN_WIDTH:
		return ReadInt(Args, &Options->PlaceChanWidth);
	case OT_FIX_PINS:
		return ReadFixPins(Args, &Options->PinFile);
	case OT_ENABLE_TIMING_COMPUTATIONS:
		return ReadOnOff(Args, &Options->ShowPlaceTiming);
	case OT_BLOCK_DIST:
		return ReadInt(Args, &Options->block_dist);

		/* Placement Options Valid Only for Timing-Driven Placement */
	case OT_TIMING_TRADEOFF:
		return ReadFloat(Args, &Options->PlaceTimingTradeoff);
	case OT_RECOMPUTE_CRIT_ITER:
		return ReadInt(Args, &Options->RecomputeCritIter);
	case OT_INNER_LOOP_RECOMPUTE_DIVIDER:
		return ReadInt(Args, &Options->inner_loop_recompute_divider);
	case OT_TD_PLACE_EXP_FIRST:
		return ReadFloat(Args, &Options->place_exp_first);
	case OT_TD_PLACE_EXP_LAST:
		return ReadFloat(Args, &Options->place_exp_last);

		/* Router Options */
	case OT_MAX_ROUTER_ITERATIONS:
		return ReadInt(Args, &Options->max_router_iterations);
	case OT_BB_FACTOR:
		return ReadInt(Args, &Options->bb_factor);
	case OT_INITIAL_PRES_FAC:
		return ReadFloat(Args, &Options->initial_pres_fac);
	case OT_PRES_FAC_MULT:
		return ReadFloat(Args, &Options->pres_fac_mult);
	case OT_ACC_FAC:
		return ReadFloat(Args, &Options->acc_fac);
	case OT_FIRST_ITER_PRES_FAC:
		return ReadFloat(Args, &Options->first_iter_pres_fac);
	case OT_BEND_COST:
		return ReadFloat(Args, &Options->bend_cost);
	case OT_ROUTE_TYPE:
		return ReadRouteType(Args, &Options->RouteType);
	case OT_VERIFY_BINARY_SEARCH:
		return Args;
	case OT_ROUTE_CHAN_WIDTH:
		return ReadInt(Args, &Options->RouteChanWidth);
	case OT_ROUTER_ALGORITHM:
		return ReadRouterAlgorithm(Args, &Options->RouterAlgorithm);
	case OT_BASE_COST_TYPE:
		return ReadBaseCostType(Args, &Options->base_cost_type);

		/* Routing options valid only for timing-driven routing */
	case OT_ASTAR_FAC:
		return ReadFloat(Args, &Options->astar_fac);
	case OT_MAX_CRITICALITY:
		return ReadFloat(Args, &Options->max_criticality);
	case OT_CRITICALITY_EXP:
		return ReadFloat(Args, &Options->criticality_exp);
	default:
		return NULL;
	}
}

/*
 * Map options set in the source t_options to a target t_options
 * structure.  Existing values in the destination have priority
 * and will not be overwritten
 */
static void MergeOptions(INOUTP t_options * dest, INP t_options * src, int id)
{
	int Token;

	for (Token = 0; Token < OT_BASE_UNKNOWN; Token++)
	{
		/* Don't override values already set in the
		 * target destination.  Also do not process
		 * Tokens that are not present in the source.
		 */
		if ((dest->Count[Token] || (!src->Count[Token])))
			continue;

		dest->Count[Token] = src->Count[Token];
		dest->Provenance[Token] = id;

		switch (Token) {
			/* File naming options */
		case OT_BLIF_FILE:
			dest->BlifFile = src->BlifFile;
			break;
		case OT_NET_FILE:
			dest->NetFile = src->NetFile;
			break;
		case OT_PLACE_FILE:
			dest->PlaceFile = src->PlaceFile;
			break;
		case OT_ROUTE_FILE:
			dest->RouteFile = src->RouteFile;
			break;
		case OT_SETTINGS_FILE:
			dest->SettingsFile = src->SettingsFile;
			break;
			/* General Options */
		case OT_NODISP:
			break;
		case OT_AUTO:
			dest->GraphPause = src->GraphPause;
			break;
		case OT_PACK:
		case OT_ROUTE:
		case OT_PLACE:
			break;
		case OT_TIMING_ANALYZE_ONLY_WITH_NET_DELAY:
			dest->constant_net_delay = src->constant_net_delay;
			break;
		case OT_FAST:
		case OT_FULL_STATS:
			break;
		case OT_TIMING_ANALYSIS:
			dest->TimingAnalysis = src->TimingAnalysis;
			break;
		case OT_OUTFILE_PREFIX:
			dest->out_file_prefix = src->out_file_prefix;
			break;
		case OT_CREATE_ECHO_FILE:
			dest->CreateEchoFile = src->CreateEchoFile;
			break;

			/* Clustering Options */
		case OT_GLOBAL_CLOCKS:
			dest->global_clocks = src->global_clocks;
			break;
		case OT_HILL_CLIMBING_FLAG:
			dest->hill_climbing_flag = src->hill_climbing_flag;
			break;
		case OT_SWEEP_HANGING_NETS_AND_INPUTS:
			dest->sweep_hanging_nets_and_inputs = src->sweep_hanging_nets_and_inputs;
			break;
		case OT_TIMING_DRIVEN_CLUSTERING:
			dest->timing_driven = src->timing_driven;
			break;
		case OT_CLUSTER_SEED:
			dest->cluster_seed_type = src->cluster_seed_type;
			break;
		case OT_ALPHA_CLUSTERING:
			dest->alpha = src->alpha;
			break;
		case OT_BETA_CLUSTERING:
			dest->beta = src->beta;
			break;
		case OT_RECOMPUTE_TIMING_AFTER:
			dest->recompute_timing_after = src->recompute_timing_after;
			break;
		case OT_CLUSTER_BLOCK_DELAY:
			dest->block_delay = src->block_delay;
			break;
		case OT_ALLOW_UNRELATED_CLUSTERING:
			dest->allow_unrelated_clustering = src->allow_unrelated_clustering;
			break;
		case OT_ALLOW_EARLY_EXIT:
			dest->allow_early_exit = src->allow_early_exit;
			break;
		case OT_INTRA_CLUSTER_NET_DELAY:
			dest->intra_cluster_net_delay = src->intra_cluster_net_delay;
			break;
		case OT_INTER_CLUSTER_NET_DELAY:
			dest->inter_cluster_net_delay = src->inter_cluster_net_delay;
			break;
		case OT_CONNECTION_DRIVEN_CLUSTERING:
			dest->connection_driven = src->connection_driven;
			break;
		case OT_SKIP_CLUSTERING:
			break;
		case OT_PACKER_ALGORITHM:
			dest->packer_algorithm = src->packer_algorithm;
			break;

			/* Placer Options */
		case OT_PLACE_ALGORITHM:
			dest->PlaceAlgorithm = src->PlaceAlgorithm;
			break;
		case OT_INIT_T:
			dest->PlaceInitT = src->PlaceInitT;
			break;
		case OT_EXIT_T:
			dest->PlaceExitT = src->PlaceExitT;
			break;
		case OT_ALPHA_T:
			dest->PlaceAlphaT = src->PlaceAlphaT;
			break;
		case OT_INNER_NUM:
			dest->PlaceInnerNum = src->PlaceInnerNum;
			break;
		case OT_SEED:
			dest->Seed = src->Seed;
			break;
		case OT_PLACE_COST_EXP:
			dest->place_cost_exp = src->place_cost_exp;
			break;
		case OT_PLACE_CHAN_WIDTH:
			dest->PlaceChanWidth = src->PlaceChanWidth;
			break;
		case OT_FIX_PINS:
			dest->PinFile = src->PinFile;
			break;
		case OT_ENABLE_TIMING_COMPUTATIONS:
			dest->ShowPlaceTiming = src->ShowPlaceTiming;
			break;
		case OT_BLOCK_DIST:
			dest->block_dist = src->block_dist;
			break;

			/* Placement Options Valid Only for Timing-Driven Placement */
		case OT_TIMING_TRADEOFF:
			dest->PlaceTimingTradeoff = src->PlaceTimingTradeoff;
			break;
		case OT_RECOMPUTE_CRIT_ITER:
			dest->RecomputeCritIter = src->RecomputeCritIter;
			break;
		case OT_INNER_LOOP_RECOMPUTE_DIVIDER:
			dest->inner_loop_recompute_divider = src->inner_loop_recompute_divider;
			break;
		case OT_TD_PLACE_EXP_FIRST:
			dest->place_exp_first = src->place_exp_first;
			break;
		case OT_TD_PLACE_EXP_LAST:
			dest->place_exp_last = src->place_exp_last;
			break;

			/* Router Options */
		case OT_MAX_ROUTER_ITERATIONS:
			dest->max_router_iterations = src->max_router_iterations;
			break;
		case OT_BB_FACTOR:
			dest->bb_factor = src->bb_factor;
			break;
		case OT_INITIAL_PRES_FAC:
			dest->initial_pres_fac = src->initial_pres_fac;
			break;
		case OT_PRES_FAC_MULT:
			dest->pres_fac_mult = src->pres_fac_mult;
			break;
		case OT_ACC_FAC:
			dest->acc_fac = src->acc_fac;
			break;
		case OT_FIRST_ITER_PRES_FAC:
			dest->first_iter_pres_fac = src->first_iter_pres_fac;
			break;
		case OT_BEND_COST:
			dest->bend_cost = src->bend_cost;
			break;
		case OT_ROUTE_TYPE:
			dest->RouteType = src->RouteType;
			break;
		case OT_VERIFY_BINARY_SEARCH:
			break;
		case OT_ROUTE_CHAN_WIDTH:
			dest->RouteChanWidth = src->RouteChanWidth;
			break;
		case OT_ROUTER_ALGORITHM:
			dest->RouterAlgorithm = src->RouterAlgorithm;
			break;
		case OT_BASE_COST_TYPE:
			dest->base_cost_type = src->base_cost_type;
			break;

			/* Routing options valid only for timing-driven routing */
		case OT_ASTAR_FAC:
			dest->astar_fac = src->astar_fac;
			break;
		case OT_MAX_CRITICALITY:
			dest->max_criticality = src->max_criticality;
			break;
		case OT_CRITICALITY_EXP:
			dest->criticality_exp = src->criticality_exp;
			break;
		default:
			break;
		}
	}
}

static char **
ReadBaseToken(INP char **Args, OUTP enum e_OptionBaseToken *Token) {
	struct s_TokenPair *Cur;

	/* Empty string is end of tokens marker */
	if (NULL == *Args)
		Error(*Args);

	/* Linear search for the pair */
	Cur = OptionBaseTokenList;
	while (Cur->Str) {
		if (strcmp(*Args, Cur->Str) == 0) {
			*Token = (enum e_OptionBaseToken) Cur->Enum;
			return ++Args;
		}
		++Cur;
	}

	*Token = OT_BASE_UNKNOWN;
	return ++Args;
}

static char **
ReadToken(INP char **Args, OUTP enum e_OptionArgToken *Token) {
	struct s_TokenPair *Cur;

	/* Empty string is end of tokens marker */
	if (NULL == *Args)
		Error(*Args);

	/* Linear search for the pair */
	Cur = OptionArgTokenList;
	while (Cur->Str) {
		if (strcmp(*Args, Cur->Str) == 0) {
			*Token = (enum e_OptionArgToken)Cur->Enum;
			return ++Args;
		}
		++Cur;
	}

	*Token = OT_ARG_UNKNOWN;
	return ++Args;
}

/* Called for parse errors. Spits out a message and then exits program. */
static void Error(INP const char *Token) {
	if (Token) {
		vpr_printf(TIO_MESSAGE_ERROR, "Unexpected token '%s' on command line\n", Token);
	} else {
		vpr_printf(TIO_MESSAGE_ERROR, "Missing token at end of command line\n");
	}
	exit(1);
}

static char **
ReadClusterSeed(INP char **Args, OUTP enum e_cluster_seed *Type) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_TIMING:
		*Type = VPACK_TIMING;
		break;
	case OT_MAX_INPUTS:
		*Type = VPACK_MAX_INPUTS;
		break;
	default:
		Error(*PrevArgs);
	}

	return Args;
}

static char **
ReadPackerAlgorithm(INP char **Args, OUTP enum e_packer_algorithm *Algo) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_GREEDY:
		*Algo = PACK_GREEDY;
		break;
	case OT_BRUTE_FORCE:
		*Algo = PACK_BRUTE_FORCE;
		break;
	default:
		Error(*PrevArgs);
	}

	return Args;
}

static char **
ReadRouterAlgorithm(INP char **Args, OUTP enum e_router_algorithm *Algo) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_BREADTH_FIRST:
		*Algo = BREADTH_FIRST;
		break;
	case OT_DIRECTED_SEARCH:
		*Algo = DIRECTED_SEARCH;
		break;
	case OT_TIMING_DRIVEN:
		*Algo = TIMING_DRIVEN;
		break;
	default:
		Error(*PrevArgs);
	}

	return Args;
}

static char **
ReadBaseCostType(INP char **Args, OUTP enum e_base_cost_type *BaseCostType) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_INTRINSIC_DELAY:
		*BaseCostType = INTRINSIC_DELAY;
		break;
	case OT_DELAY_NORMALIZED:
		*BaseCostType = DELAY_NORMALIZED;
		break;
	case OT_DEMAND_ONLY:
		*BaseCostType = DEMAND_ONLY;
		break;
	default:
		Error(*PrevArgs);
	}

	return Args;
}

static char **
ReadRouteType(INP char **Args, OUTP enum e_route_type *Type) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_GLOBAL:
		*Type = GLOBAL;
		break;
	case OT_DETAILED:
		*Type = DETAILED;
		break;
	default:
		Error(*PrevArgs);
	}

	return Args;
}

static char **
ReadPlaceAlgorithm(INP char **Args, OUTP enum e_place_algorithm *Algo) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_BOUNDING_BOX:
		*Algo = BOUNDING_BOX_PLACE;
		break;
	case OT_NET_TIMING_DRIVEN:
		*Algo = NET_TIMING_DRIVEN_PLACE;
		break;
	case OT_PATH_TIMING_DRIVEN:
		*Algo = PATH_TIMING_DRIVEN_PLACE;
		break;
	default:
		Error(*PrevArgs);
	}

	return Args;
}

static char **
ReadFixPins(INP char **Args, OUTP char **PinFile) {
	enum e_OptionArgToken Token;
	int Len;
	char **PrevArgs = Args;

	Args = ReadToken(Args, &Token);
	if (OT_RANDOM != Token) {
		Len = 1 + strlen(*PrevArgs);
		*PinFile = (char *) my_malloc(Len * sizeof(char));
		memcpy(*PinFile, *PrevArgs, Len);
	}
	return Args;
}

static char **
ReadOnOff(INP char **Args, OUTP boolean * Val) {
	enum e_OptionArgToken Token;
	char **PrevArgs;

	PrevArgs = Args;
	Args = ReadToken(Args, &Token);
	switch (Token) {
	case OT_ON:
		*Val = TRUE;
		break;
	case OT_OFF:
		*Val = FALSE;
		break;
	default:
		Error(*PrevArgs);
	}
	return Args;
}

static char **
ReadInt(INP char **Args, OUTP int *Val) {
	if (NULL == *Args)
		Error(*Args);
	if ((**Args > '9') || (**Args < '0'))
		Error(*Args);

	*Val = atoi(*Args);

	return ++Args;
}

static char **
ReadFloat(INP char ** Args, OUTP float *Val) {
	if (NULL == *Args) {
		Error(*Args);
	}

	if ((**Args != '-') && (**Args != '.')
			&& ((**Args > '9') || (**Args < '0'))) {
		Error(*Args);
	}

	*Val = atof(*Args);

	return ++Args;
}

static char **
ReadString(INP char **Args, OUTP char **Val) {
	if (NULL == *Args) {
		Error(*Args);
	}

	*Val = my_strdup(*Args);

	return ++Args;
}
