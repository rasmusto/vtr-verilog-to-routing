#ifndef PATH_DELAY
#define PATH_DELAY

float **alloc_and_load_timing_graph(t_timing_inf timing_inf);

float **alloc_and_load_pre_packing_timing_graph(float block_delay,
		float inter_cluster_net_delay, t_model *models, t_timing_inf timing_inf);

t_linked_int *allocate_and_load_critical_path(void);

void load_timing_graph_net_delays(float **net_delay);

float load_net_slack(float **net_slack, boolean do_lut_input_balancing);

void free_timing_graph(float **net_slack);

void print_timing_graph(const char *fname);

void print_net_slack(const char *fname, float **net_slack);

void print_lut_remapping(const char *fname);
void print_critical_path(const char *fname);

void get_tnode_block_and_output_net(int inode, int *iblk_ptr, int *inet_ptr);

void do_constant_net_delay_timing_analysis(t_timing_inf timing_inf,
		float constant_net_delay_value);

void print_timing_graph_as_blif(const char *fname, t_model *models);

extern int num_netlist_clocks; /* [0..num_netlist_clocks - 1] number of clocks in netlist */
extern t_clock * clock_list; /* [0..num_netlist_clocks - 1] array of clocks in netlist */
extern float ** timing_constraints; /* [0..num_netlist_clocks - 1 (source)][0..num_netlist_clocks - 1 (destination)] */

#endif
