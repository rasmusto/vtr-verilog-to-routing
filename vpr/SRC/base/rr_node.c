#include "rr_node.h"

/* Create an array of strings which maps to an enumeration (t_rr_type)  *
 * so a routing resource name can be identified 						*
 * by its index in the array. To retrieve desired rr-type name, call    *
 * member function rr_get_type_string() of the structure s_rr_node.     */
static const char *name_type[] = { "SOURCE", "SINK", "IPIN", "OPIN", "CHANX", 
							"CHANY", "INTRA_CLUSTER_EDGE" };

/* Member function of "struct s_rr_node" used to retrieve a routing *
 * resource type string by its index, which is defined by           *
 * "t_rr_type type".												*/
const char *t_rr_node::rr_get_type_string() const {
	return name_type[type];
}

short t_rr_node::get_xlow() const {
	return xlow;
}

short t_rr_node::get_ylow() const {
	return ylow;
}

short t_rr_node::get_xhigh() const {
	return xhigh;
}

short t_rr_node::get_yhigh() const {
	return yhigh;
}

short t_rr_node::get_ptc_num() const {
	return ptc_num;
}

short t_rr_node::get_cost_index() const {
	return cost_index;
}

short t_rr_node::get_capacity() const {
	return capacity;
}

short t_rr_node::get_fan_in() const{
	return fan_in;
}

short t_rr_node::get_num_edges() const {
	return num_edges;
}

void t_rr_node::set_xlow(short _xlow) {
	xlow = _xlow;
}

void t_rr_node::set_ylow(short _ylow) {
	ylow = _ylow;
}

void t_rr_node::set_xhigh(short _xhigh) {
	xhigh = _xhigh;
}

void t_rr_node::set_yhigh(short _yhigh) {
	yhigh = _yhigh;
}

void t_rr_node::set_ptc_num(short _ptc_num) {
	ptc_num = _ptc_num;
}

void t_rr_node::set_cost_index(short _cost_index) {
	cost_index = _cost_index;
}

void t_rr_node::set_capacity(short _capacity) {
	capacity = _capacity;
}

void t_rr_node::set_fan_in(short _fan_in) {
	fan_in = _fan_in;
}

void t_rr_node::set_num_edges(short _num_edges) {
	num_edges = _num_edges;
}
