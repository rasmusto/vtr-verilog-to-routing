#ifndef PATH_DELAY
#define PATH_DELAY

#define DO_NOT_ANALYSE -1

/*********************** Defines for timing options *******************************/

#define SLACK_DEFINITION 'R'
/* Choose how to normalize negative slacks for the optimizers (not in the final timing analysis for output statistics):
   'R' (T_req-relaxed): For each constraint, set the required time at sink nodes to the max of the true required time 
	   (constraint + tnode[inode].clock_skew) and the max arrival time. This means that the required time is "relaxed" 
	   to the max arrival time for tight constraints which would otherwise	give negative slack.
	   Criticalities are computed once per constraint, using a criticality denominator unique to that constraint
	   (maximum of the constraint and the max arrival time).
   'S' (Shifted): After all slacks are computed, increase the value of all slacks by the largest negative slack, 
       if it exists. Equivalent to 'R' for single-clock cases. 
	   Criticalities are computed once per timing analysis, using a single criticality denominator for all constraints
	   (maximum of all constraints and all required times).
	   This can give unusual results with multiple, very dissimilar constraints.
*/

/*************************** Function declarations ********************************/

t_slack * alloc_and_load_timing_graph(t_timing_inf timing_inf);

t_slack * alloc_and_load_pre_packing_timing_graph(float block_delay,
		float inter_cluster_net_delay, t_model *models, t_timing_inf timing_inf);

t_linked_int *allocate_and_load_critical_path(void);

void load_timing_graph_net_delays(float **net_delay);

void do_timing_analysis(t_slack * slacks, boolean is_prepacked, boolean do_lut_input_balancing, boolean is_final_analysis);

void free_timing_graph(t_slack * slack);

void free_timing_stats(void);

void print_timing_graph(const char *fname);

void print_lut_remapping(const char *fname);

void print_slack(float ** slack, boolean slack_is_normalized, const char *fname);

void print_criticality(float ** criticality, boolean criticality_is_normalized, const char *fname);

void print_net_delay(float **net_delay, const char *fname);
#ifdef PATH_COUNTING
void print_path_weight(float ** path_weight, const char *fname);
#else
void print_clustering_timing_info(const char *fname);
#endif
void print_timing_stats(void);

float get_critical_path_delay(void);

void print_critical_path(const char *fname);

void get_tnode_block_and_output_net(int inode, int *iblk_ptr, int *inet_ptr);

void do_constant_net_delay_timing_analysis(t_timing_inf timing_inf,
		float constant_net_delay_value);

void print_timing_graph_as_blif (const char *fname, t_model *models);

/*************************** Variable declarations ********************************/

extern int num_constrained_clocks; /* number of clocks with timing constraints */
extern t_clock * constrained_clocks; /* [0..num_constrained_clocks - 1] array of clocks with timing constraints */

extern int num_constrained_inputs; /* number of inputs with timing constraints */
extern t_io * constrained_inputs; /* [0..num_constrained_inputs - 1] array of inputs with timing constraints */

extern int num_constrained_outputs; /* number of outputs with timing constraints */
extern t_io * constrained_outputs; /* [0..num_constrained_outputs - 1] array of outputs with timing constraints */

extern float ** timing_constraint; /* [0..num_constrained_clocks - 1 (source)][0..num_constrained_clocks - 1 (sink)] */

extern int num_cc_constraints; /* number of special-case clock-to-clock constraints overriding default, calculated, timing constraints */
extern t_override_constraint * cc_constraints; /*  [0..num_cc_constraints - 1] array of such constraints */

extern int num_cf_constraints; /* number of special-case clock-to-flipflop constraints */
extern t_override_constraint * cf_constraints; /*  [0..num_cf_constraints - 1] array of such constraints */

extern int num_fc_constraints; /* number of special-case flipflop-to-clock constraints */
extern t_override_constraint * fc_constraints; /*  [0..num_fc_constraints - 1] */

extern int num_ff_constraints; /* number of special-case flipflop-to-flipflop constraints */
extern t_override_constraint * ff_constraints; /*  [0..num_ff_constraints - 1] array of such constraints */

#endif
