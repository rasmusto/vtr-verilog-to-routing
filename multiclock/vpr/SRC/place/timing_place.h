#ifndef TIMING_PLACE
#define TIMING_PLACE

void alloc_lookups_and_criticalities(t_chan_width_dist chan_width_dist,
		struct s_router_opts router_opts,
		struct s_det_routing_arch det_routing_arch, t_segment_inf * segment_inf,
		t_timing_inf timing_inf, float ***net_delay);

void free_lookups_and_criticalities(float ***net_delay);

void print_sink_delays(char *fname);

void print_sink_delays(const char *fname);
void load_criticalities(float crit_exponent);

extern float **timing_place_crit;

#endif