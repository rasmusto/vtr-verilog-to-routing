#include "vpr_types.h"

#ifndef RR_NODE_H
#define RR_NODE_H


/* Main structure describing one routing resource node.  Everything in       *
 * this structure should describe the graph -- information needed only       *
 * to store algorithm-specific data should be stored in one of the           *
 * parallel rr_node_? structures.                                            *
 *                                                                           *
 * xlow, xhigh, ylow, yhigh:  Integer coordinates (see route.c for           *
 *       coordinate system) of the ends of this routing resource.            *
 *       xlow = xhigh and ylow = yhigh for pins or for segments of           *
 *       length 1.  These values are used to decide whether or not this      *
 *       node should be added to the expansion heap, based on things         *
 *       like whether it's outside the net bounding box or is moving         *
 *       further away from the target, etc.                                  *
 * type:  What is this routing resource?                                     *
 * ptc_num:  Pin, track or class number, depending on rr_node type.          *
 *           Needed to properly draw.                                        *
 * cost_index: An integer index into the table of routing resource indexed   *
 *             data (this indirection allows quick dynamic changes of rr     *
 *             base costs, and some memory storage savings for fields that   *
 *             have only a few distinct values).                             *
 * occ:        Current occupancy (usage) of this node.                       *
 * capacity:   Capacity of this node (number of routes that can use it).     *
 * num_edges:  Number of edges exiting this node.  That is, the number       *
 *             of nodes to which it connects.                                *
 * edges[0..num_edges-1]:  Array of indices of the neighbours of this        *
 *                         node.                                             *
 * switches[0..num_edges-1]:  Array of switch indexes for each of the        *
 *                            edges leaving this node.                       *
 *                                                                           *
 * The following parameters are only needed for timing analysis.             *
 * R:  Resistance to go through this node.  This is only metal               *
 *     resistance (end to end, so conservative) -- it doesn't include the    *
 *     switch that leads to another rr_node.                                 *
 * C:  Total capacitance of this node.  Includes metal capacitance, the      *
 *     input capacitance of all switches hanging off the node, the           *
 *     output capacitance of all switches to the node, and the connection    *
 *     box buffer capacitances hanging off it.                               *
 * (UDSD by AY) direction: if the node represents a track, this field        *
 *                         indicates the direction of the track. Otherwise   *
 *                         the value contained in the field should be        *
 *                         ignored.                                          *
 * (UDSD by AY) drivers: if the node represents a track, this field          *
 *                       indicates the driving architecture of the track.    *
 *                       Otherwise the value contained in the field should   *
 *                       be ignored.                                         */

#define NO_PREVIOUS -1

class t_rr_node {
	
	short xlow;
	short xhigh;
	short ylow;
	short yhigh;

public:
	short ptc_num;

	short cost_index;
	short occ;
	short capacity;
	short fan_in;
	short num_edges;
	t_rr_type type;
	int *edges;
	short *switches;

	float R;
	float C;

	enum e_direction direction; /* UDSD by AY */
	enum e_drivers drivers; /* UDSD by AY */
	int num_wire_drivers; /* UDSD by WMF */
	int num_opin_drivers; /* UDSD by WMF (could use "short") */

	/* Used by clustering only (TODO, may wish to extend to regular router) */
	int prev_node;
	int prev_edge;
	int net_num;
	t_pb_graph_pin *pb_graph_pin;
	t_tnode *tnode;
	float pack_intrinsic_cost;
	/* For IPIN, source, and sink nodes, helps identify which location this 
	rr_node belongs to */
	int z; 
	
	/* Member functions */
	const char *rr_get_type_string(); /* Retrieve rr_type as a string */
	short get_xlow() const;
	short get_ylow() const;
	short get_xhigh() const;
	short get_yhigh() const;
	void set_xlow(short);
	void set_ylow(short);
	void set_xhigh(short);
	void set_yhigh(short);
};


/* Data that is pointed to by the .cost_index member of t_rr_node.  It's     *
 * purpose is to store the base_cost so that it can be quickly changed       *
 * and to store fields that have only a few different values (like           *
 * seg_index) or whose values should be an average over all rr_nodes of a    *
 * certain type (like T_linear etc., which are used to predict remaining     *
 * delay in the timing_driven router).                                       *
 *                                                                           *
 * base_cost:  The basic cost of using an rr_node.                           *
 * ortho_cost_index:  The index of the type of rr_node that generally        *
 *                    connects to this type of rr_node, but runs in the      *
 *                    orthogonal direction (e.g. vertical if the direction   *
 *                    of this member is horizontal).                         *
 * seg_index:  Index into segment_inf of this segment type if this type of   *
 *             rr_node is an CHANX or CHANY; OPEN (-1) otherwise.            *
 * inv_length:  1/length of this type of segment.                            *
 * T_linear:  Delay through N segments of this type is N * T_linear + N^2 *  *
 *            T_quadratic.  For buffered segments all delay is T_linear.     *
 * T_quadratic:  Dominant delay for unbuffered segments, 0 for buffered      *
 *               segments.                                                   *
 * C_load:  Load capacitance seen by the driver for each segment added to    *
 *          the chain driven by the driver.  0 for buffered segments.        */

typedef struct s_rr_indexed_data {
	float base_cost;
	float saved_base_cost;
	int ortho_cost_index;
	int seg_index;
	float inv_length;
	float T_linear;
	float T_quadratic;
	float C_load;

	/* Power Estimation: Wire capacitance in (Farads * tiles / meter)
	 * This is used to calculate capacitance of this segment, by
	 * multiplying it by the length per tile (meters/tile).
	 * This is only the wire capacitance, not including any switches */
	float C_tile_per_m;
} t_rr_indexed_data;

#endif
