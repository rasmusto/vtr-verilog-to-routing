/* This file holds subroutines responsible for drawing inside clustered logic blocks.
 * The four main subroutines defined here are draw_internal_alloc_blk(), 
 * draw_internal_init_blk(), draw_internal_draw_subblk(), and toggle_blk_internal().
 * When VPR graphics initially sets up, draw_internal_alloc_blk() will be called from 
 * draw.c to allocate space for the structures needed for internal blks drawing.
 * Before any drawing, draw_internal_init_blk() will pre-compute a bounding box
 * for each sub-block in the pb_graph of every physical block type. When the menu button 
 * "Blk Internal" is pressed, toggle_blk_internal() will enable internal blocks drawing.
 * Then, with each subsequent click on the button, toggle_blk_internal() will propel one 
 * more level of pbs to be drawn. Draw_internal_draw_subblk() will be called whenever 
 * new blocks need to be drawn, and this function is responsible for drawing sub-blocks 
 * from the pre-computed bounding box values.
 * 
 * Author: Long Yu (Mike) Wang
 * Date: August 2013, May 2014
 *
 * Author: Matthew J.P. Walker
 * Date: May 2014
 */

#include <cstdio>
#include <algorithm>
#include <assert.h>
#include <string.h>
using namespace std;

#include "intra_logic_block.h"
#include "globals.h"
#include "vpr_utils.h"
#include "draw_global.h"
#include "graphics.h"
#include "draw.h"

/************************* Subroutines local to this file. *******************************/

static void draw_internal_load_coords(int type_descrip_index, t_pb_graph_node *pb_graph_node, 
									  float parent_width, float parent_height);
static void draw_internal_calc_coords(int type_descrip_index, t_pb_graph_node *pb_graph_node, 
									  int num_pb_types, int type_index, int num_pb, int pb_index, 
									  float parent_width, float parent_height, 
									  OUTP float *blk_width, OUTP float *blk_height);
static int draw_internal_find_max_lvl(t_pb_type pb_type);
static void draw_internal_pb(const t_block* const clb, t_pb* pb, const t_bound_box& parent_bbox);
static bool is_top_lvl_block_highlighted(const t_block& clb);

static float calc_text_xbound(const t_bound_box& center_of, char* const text);
static float calc_text_xbound(float start_x, float start_y, float end_x, float end_y, char* const text);

static void draw_logical_connections_of(t_pb* pb, t_block* clb);

void draw_one_logical_connection(
	const t_net_pin& src_pin,  const t_logical_block& src_lblk, const t_bound_box& src_abs_bbox,
	const t_net_pin& sink_pin, const t_logical_block& sink_lblk, const t_bound_box& sink_abs_bbox);
static t_pb* highlight_sub_block_helper(t_block* clb, t_pb* pb, t_point local_pt, int max_depth);

/************************* Subroutine definitions begin *********************************/

void draw_internal_alloc_blk() {
	t_draw_coords *draw_coords;
	int i;
	t_pb_graph_node *pb_graph_head;
	
	/* Call accessor function to retrieve global variables. */
	draw_coords = get_draw_coords_vars();
	
	/* Create a vector holding coordinate information for each type of physical logic
	 * block.
	 */
	draw_coords->blk_info.resize(num_types);

	for (i = 0; i < num_types; ++i) {
		/* Empty block has no sub_blocks */
		if (&type_descriptors[i] == EMPTY_TYPE)
			continue;
		
		pb_graph_head = type_descriptors[i].pb_graph_head;
		
		/* Create an vector with size equal to the total number of pins for each type 
		 * of physical logic block, in order to uniquely identify each sub-block in 
		 * the pb_graph of that type.
		 */
		draw_coords->blk_info.at(i).subblk_array.resize(pb_graph_head->total_pb_pins);
	}
}


void draw_internal_init_blk() {
	/* Call accessor function to retrieve global variables. */
	t_draw_coords* draw_coords = get_draw_coords_vars();
	t_draw_state* draw_state = get_draw_state_vars();

	t_pb_graph_node *pb_graph_head_node;

	for (int i = 0; i < num_types; ++i) {
		/* Empty block has no sub_blocks */
		if (&type_descriptors[i] == EMPTY_TYPE)
			continue;

		pb_graph_head_node = type_descriptors[i].pb_graph_head;
		int type_descriptor_index = type_descriptors[i].index;

		int num_sub_tiles = type_descriptors[i].capacity;

		float clb_width = type_descriptors[i].width * draw_coords->tile_width
					/num_sub_tiles;
		float clb_height = type_descriptors[i].height * draw_coords->tile_width;

		// set the clb dimensions
		t_bound_box& clb_bbox = draw_coords->blk_info.at(type_descriptor_index).subblk_array.at(0);

		clb_bbox.bottom_left().set(0,0);
		clb_bbox.top_right().set(clb_bbox.bottom_left());
		clb_bbox.top_right().offset(clb_width, clb_height);

		draw_internal_load_coords(type_descriptor_index, pb_graph_head_node, 
								  clb_width, clb_height);

		/* Determine the max number of sub_block levels in the FPGA */
		draw_state->max_sub_blk_lvl = max(draw_internal_find_max_lvl(*type_descriptors[i].pb_type),
							   draw_state->max_sub_blk_lvl);
	}
}



void draw_internal_draw_subblk() {
	for (int i = 0; i <= (nx + 1); i++) {
		for (int j = 0; j <= (ny + 1); j++) {
			/* Only the first block of a group should control drawing */
			if (grid[i][j].width_offset > 0 || grid[i][j].height_offset > 0) 
				continue;

			/* Don't draw if tile is empty. This includes corners. */
			if (grid[i][j].type == EMPTY_TYPE)
				continue;

			int num_sub_tiles = grid[i][j].type->capacity;
			for (int k = 0; k < num_sub_tiles; ++k) {
				/* Don't draw if block is empty. */
				if (grid[i][j].blocks[k] == EMPTY || grid[i][j].blocks[k] == INVALID)
					continue;

				/* Get block ID */
				int bnum = grid[i][j].blocks[k];
				/* Safety check, that physical blocks exists in the CLB */
				if (block[bnum].pb == NULL)
					continue;

				draw_internal_pb(&block[bnum], block[bnum].pb, t_bound_box(0,0,0,0));
			}
		}
	}
}


/* This function traverses through the pb_graph of a certain physical block type and 
 * finds the maximum sub-block levels for that type.
 */
static int draw_internal_find_max_lvl(t_pb_type pb_type) {
	int i, j;
	t_mode mode;
	int max_levels = 0;

	/* If no modes, we have reached the end of pb_graph */
	if (pb_type.num_modes == 0)
		return (pb_type.depth);

	for (i = 0; i < pb_type.num_modes; ++i) {
		mode = pb_type.modes[i];

		for (j = 0; j < mode.num_pb_type_children; ++j) {
			max_levels = max(draw_internal_find_max_lvl(mode.pb_type_children[j]), max_levels);
		}
	}

	return max_levels;
}


/* Helper function for initializing bounding box values for each sub-block. This function
 * traverses through the pb_graph for a descriptor_type (given by type_descrip_index), and
 * calls helper function to compute bounding box values.
 */
static void draw_internal_load_coords(int type_descrip_index, t_pb_graph_node *pb_graph_node, 
									  float parent_width, float parent_height) {
	int i, j, k;
	t_pb_type *pb_type;
	int num_modes, num_children, num_pb;
	t_mode mode;
	float blk_width = 0.;
	float blk_height = 0.;

	/* Get information about the pb_type */
	pb_type = pb_graph_node->pb_type;
	num_modes = pb_type->num_modes;

	/* If no modes, we have reached the end of pb_graph */
	if (num_modes == 0)
		return;

	for (i = 0; i < num_modes; ++i) {
		mode = pb_type->modes[i];
		num_children = mode.num_pb_type_children;

		for (j = 0; j < num_children; ++j) {
			/* Find the number of instances for each child pb_type. */
			num_pb = mode.pb_type_children[j].num_pb;

			for (k = 0; k < num_pb; ++k) { 
				/* Compute bound box for block. Don't call if pb_type is root-level pb. */
				draw_internal_calc_coords(type_descrip_index, 
										  &pb_graph_node->child_pb_graph_nodes[i][j][k], 
										  num_children, j, num_pb, k,
										  parent_width, parent_height, 
										  &blk_width, &blk_height);		

				/* Traverse to next level in the pb_graph */
				draw_internal_load_coords(type_descrip_index, 
										  &pb_graph_node->child_pb_graph_nodes[i][j][k], 
										  blk_width, blk_height);
			}
		}
	}
	return;
}


/* Helper function which computes bounding box values for a sub-block. The coordinates 
 * are relative to the left and bottom corner of the parent block.
 */
static void 
draw_internal_calc_coords(int type_descrip_index, t_pb_graph_node *pb_graph_node, 
						  int num_pb_types, int type_index, int num_pb, int pb_index, 
						  float parent_width, float parent_height, 
						  OUTP float *blk_width, OUTP float *blk_height) 
{
	float parent_drawing_width, parent_drawing_height;
	float sub_tile_x, sub_tile_y;
	float child_width, child_height;
	// get the bbox for this pb type
	t_bound_box& pb_bbox = get_draw_coords_vars()->blk_info.at(type_descrip_index).get_pb_bbox_ref(*pb_graph_node);

	const float FRACTION_PARENT_PADDING_X = 0.01;

	const float NORMAL_FRACTION_PARENT_HEIGHT = 0.90;
	      float capacity_divisor = 1;
	const float FRACTION_PARENT_PADDING_BOTTOM = 0.01;

	const float FRACTION_CHILD_MARGIN_X = 0.025;
	const float FRACTION_CHILD_MARGIN_Y = 0.04;

	int capacity = type_descriptors[type_descrip_index].capacity;
	if (capacity > 1 && nx > 0 && ny > 0 && grid[1][0].usage != 0
		&& type_descrip_index == grid[1][0].type->index) {

		// that should test for io blocks, and setting capacity_divisor > 1
		// will squish every thing down
		capacity_divisor = capacity - 1;
	}

	/* Draw all child-level blocks in just most of the space inside their parent block. */
	parent_drawing_width = parent_width * (1 - FRACTION_PARENT_PADDING_X*2);
	parent_drawing_height = parent_height * (NORMAL_FRACTION_PARENT_HEIGHT / capacity_divisor);


	/* The left and bottom corner (inside the parent block) of the space to draw 
	 * child blocks. 
	 */
	sub_tile_x = parent_width * FRACTION_PARENT_PADDING_X;
	sub_tile_y = parent_height * FRACTION_PARENT_PADDING_BOTTOM;

	/* Divide parent_drawing_width by the number of child types. */
	child_width = parent_drawing_width/num_pb_types;
	/* Divide parent_drawing_height by the number of instances of the pb_type. */
	child_height = parent_drawing_height/num_pb;

	/* The starting point to draw the physical block. */
	pb_bbox.left()   = child_width * type_index + sub_tile_x + FRACTION_CHILD_MARGIN_X * child_width;
	pb_bbox.bottom() = child_height * pb_index  + sub_tile_y + FRACTION_CHILD_MARGIN_Y * child_height;

	/* Leave some space between different pb_types. */
	child_width *= 1 - FRACTION_CHILD_MARGIN_X*2;
	/* Leave some space between different instances of the same type. */
	child_height *= 1 - FRACTION_CHILD_MARGIN_Y*2;

	/* Endpoint for drawing the pb_type */
	pb_bbox.right() = pb_bbox.left()   + child_width;
	pb_bbox.top()   = pb_bbox.bottom() + child_height;

	*blk_width = child_width;
	*blk_height = child_height;

	return;
}

/* Helper subroutine to draw all sub-blocks. This function traverses through the pb_graph 
 * which a netlist block can map to, and draws each sub-block inside its parent block. With
 * each click on the "Blk Internal" button, a new level is shown. 
 */
static void draw_internal_pb(const t_block* const clb, t_pb* pb, const t_bound_box& parent_bbox) {
	t_draw_coords* draw_coords = get_draw_coords_vars();
	t_draw_state* draw_state = get_draw_state_vars();
	t_selected_sub_block_info& sel_sub_info = get_selected_sub_block_info();

	t_pb_type* pb_type = pb->pb_graph_node->pb_type;
	t_bound_box abs_bbox = draw_coords->get_pb_bbox(*clb, *pb->pb_graph_node) + parent_bbox.bottom_left();

	// if we've gone too far, don't draw anything
	if (pb_type->depth > draw_state->show_blk_internal) {
		return;
	}

	/// first draw box ///

	if (pb_type->depth == 0) {
		if (!is_top_lvl_block_highlighted(*clb)) {
			// if this is a top level pb, and only if it isn't selected (ie. a funny colour),
			// overwrite it. (but stil draw the text)
			setcolor(WHITE);
			fillrect(abs_bbox); 
			setcolor(BLACK);
			setlinestyle(SOLID);
			drawrect(abs_bbox);
		}
	} else {
		if (pb->name != NULL) {
			// If block is used, draw it in colour with solid border.

			setlinestyle(SOLID);

			// type_index indicates what type of block.
			const int type_index = clb->type->index;

			// determine default background color
			if (sel_sub_info.is_selected(pb->pb_graph_node, clb)) {
				setcolor(SELECTED_COLOR);
			} else if (sel_sub_info.is_sink_of_selected(pb->pb_graph_node, clb)) {
				setcolor(DRIVES_IT_COLOR);
			} else if (sel_sub_info.is_source_of_selected(pb->pb_graph_node, clb)) {
				setcolor(DRIVEN_BY_IT_COLOR);
			} else if (pb_type->depth != draw_state->show_blk_internal && pb->child_pbs != NULL) {
				setcolor(WHITE); // draw anthing that will have a child as white
			} else if (type_index < 3) {
				setcolor(LIGHTGREY);
			} else if (type_index < 3 + MAX_BLOCK_COLOURS) {
				setcolor(BISQUE + MAX_BLOCK_COLOURS + type_index - 3);
			} else {
				setcolor(BISQUE + 2 * MAX_BLOCK_COLOURS - 1);
			}
		}
		else {
			// If block is not used, draw as empty block (ie. white
			// background with dashed border).

			setlinestyle(DASHED);
			setcolor(WHITE);
		}

		fillrect(abs_bbox);
		setcolor(BLACK);
		drawrect(abs_bbox);
	}

	/// then draw text ///

	if (pb->name != NULL) {
		setfontsize(16); // note: calc_text_xbound(...) assumes this is 16
		if (pb_type->depth == draw_state->show_blk_internal || pb->child_pbs == NULL) {
			// If this pb is at the lowest displayed level, or has no more children, then
			// label it in the center with its type and name

			int type_len = strlen(pb_type->name);
			int name_len = strlen(pb->name);
			int tot_len = type_len + name_len;
			char* blk_tag = (char *)my_malloc((tot_len + 8) * sizeof(char));

			sprintf (blk_tag, "%s(%s)", pb_type->name, pb->name);

			drawtext(abs_bbox.get_xcenter(), abs_bbox.get_ycenter(),
				 blk_tag, calc_text_xbound(abs_bbox, blk_tag));

			free(blk_tag);
		} else {
			// else (ie. has chilren, and isn't at the lowest displayed level)
			// just label its type, and put it up at the top so we can see it
			// Note: Here, I am tricking the function into using a slightly different weighting because
			// the text is very close to the top of the block.
			drawtext(abs_bbox.get_xcenter(), abs_bbox.top() - (abs_bbox.get_height()) / 15.0,
		        pb_type->name, calc_text_xbound(abs_bbox.left(), abs_bbox.bottom() / 5.0f,
				                                abs_bbox.right(), abs_bbox.top() / 5.0f, pb_type->name)
			);
		}
	} else {
		// If child block is not used, label it only by its type
		drawtext(abs_bbox.get_xcenter(), abs_bbox.get_ycenter(),
			 pb_type->name, calc_text_xbound(abs_bbox, pb_type->name));
	}

	/// now recurse on the child pbs. ///

	// return if no children, or this is an unusused pb,
	// or if going down will be too far down (this one is redundant, but for optimazition)
	if(pb->child_pbs == NULL || pb->name == NULL
		|| pb_type->depth == draw_state->show_blk_internal) {

		return;
	}

	int num_child_types = pb->get_num_child_types();
	for (int i = 0; i < num_child_types; ++i) {

		if (pb->child_pbs[i] == NULL) {
			continue;
		}

		int num_pb = pb->get_num_children_of_type(i);
		for (int j = 0; j < num_pb; ++j) {

			t_pb* child_pb = &pb->child_pbs[i][j];

			// don't go farther if null 
			if (child_pb == NULL) {
				continue;
			}

			t_pb_type* pb_child_type = child_pb->pb_graph_node->pb_type;

			// don't go farther if 0 modes
			if (pb_child_type == NULL && pb_child_type->num_modes == 0) {
				continue;
			}

			// now recurse
			draw_internal_pb(clb, child_pb, abs_bbox);

		}
	}
}

void draw_logical_connections() {
	const t_selected_sub_block_info& sel_sub_info = get_selected_sub_block_info();
	t_draw_state* draw_state = get_draw_state_vars();
	// t_draw_coords *draw_coords = get_draw_coords_vars();

	if (draw_state->show_nets == DRAW_LOGICAL_CONNECTIONS) {
		draw_logical_connections_of(NULL,NULL);
	}

	if (sel_sub_info.has_selection()) {

		t_pb* pb = sel_sub_info.get_selected_pb();
		t_block* clb = sel_sub_info.get_containing_block();

		draw_logical_connections_of(pb,clb);
	}
}

/**
 * Draws the logical connections of the given pb in clb, and all of it's
 * children, and children's children and so on. pb must be in clb, and in 
 * fact could be determined from pb, but is generally available where this
 * function is called. 
 *
 * *NOTE: If pb or clb is null, then this function will not recurse, and instead
 *        will draw ALL logical connections of ALL logical blocks.
 */
static void draw_logical_connections_of(t_pb* pb, t_block* clb) {
	
	if (pb != NULL && clb != NULL && pb->child_pbs != 0) {
		int num_child_types = pb->get_num_child_types();
		for (int i = 0; i < num_child_types; ++i) {
				
			int num_pb = pb->get_num_children_of_type(i);
			for (int j = 0; j < num_pb; ++j) {	

				draw_logical_connections_of(&pb->child_pbs[i][j], clb);

			}
		}
		return;
	}

	// iterate over all the atom nets
	for (vector<t_vnet>::iterator net = g_atoms_nlist.net.begin(); net != g_atoms_nlist.net.end(); ++net){

		int src_lblk_id = net->pins.at(0).block;
		t_logical_block* src_lblk = &logical_block[src_lblk_id];
		t_pb_graph_node* src_pb_gnode = src_lblk->pb->pb_graph_node;

		// get the abs bbox of of the driver pb
		const t_bound_box& src_bbox = get_draw_coords_vars()->get_absolute_pb_bbox(src_lblk->clb_index, src_pb_gnode);

		// iterate over the sinks
		for (std::vector<t_net_pin>::iterator pin = net->pins.begin() + 1;
			pin != net->pins.end(); ++pin) {

			t_logical_block* sink_lblk = &logical_block[pin->block];
			t_pb_graph_node* sink_pb_gnode = sink_lblk->pb->pb_graph_node;

			if (pb == NULL || clb == NULL) {
				setcolor(BLACK);
			} else {
				if (src_pb_gnode == pb->pb_graph_node && clb == &block[src_lblk->clb_index]) {
					setcolor(DRIVES_IT_COLOR);
				} else if (sink_pb_gnode == pb->pb_graph_node && clb == &block[sink_lblk->clb_index]) {
					setcolor(DRIVEN_BY_IT_COLOR);
				} else {
					continue; // not showing all, and not the sperified block, so skip
				}
			}

			// get the abs. bbox of the sink pb
			const t_bound_box& sink_bbox = get_draw_coords_vars()->get_absolute_pb_bbox(sink_lblk->clb_index, sink_pb_gnode);

			draw_one_logical_connection(net->pins.at(0), *src_lblk, src_bbox, *pin, *sink_lblk, sink_bbox);		
		}
	}
}

/**
 * Helper function for draw_one_logical_connection(...). Finds the index of a pin
 * in the given model. It counts pins in the model until it finds the given pin,
 * and puts this count in pin_index. The total number of pins is also calculated
 * and put in total_pins. The search inpts parameter tells this function to search
 * inpts (if true) or outputs (if false).
 */
void find_pin_index_at_model_scope(
	const t_net_pin& the_pin, const t_logical_block& lblk, const bool search_inputs,
	OUTP int* pin_index, OUTP int* total_pins) {

	// printf("looking for pin {blk: %d, port: %d, pin: %d}\n",
		// the_pin.block, the_pin.block_port, the_pin.block_pin);

	t_model_ports* port = NULL;
	if (search_inputs) {
		port = lblk.model->inputs;
	} else { // searching outputs
		port = lblk.model->outputs;
	}

	*pin_index = -1;
	int i = 0;
	// Note, that all iterations must go through - no early exits -
	// as we are also trying to count the total number of pins

	// iterate over the ports.
	while(port != NULL) {
		if(search_inputs ? port->is_clock == FALSE : true) {
			int iport = port->index;
			// iterate over the pins on that port
			// printf("looking at port %d\n", iport);
			for (int ipin = 0; ipin < port->size; ipin++) {
				// get the net that connects here.
				int inet = OPEN;
				if (search_inputs) {
					inet = lblk.input_nets[iport][ipin];
				} else {
					inet = lblk.output_nets[iport][ipin];
				}
				if(inet != OPEN) {
					// printf("looking at net %d\n", inet);
					t_vnet& net = g_atoms_nlist.net.at(inet);
					for (auto pin = net.pins.begin(); pin != net.pins.end(); ++pin) {
						// printf("looking at pin {blk: %d, port: %d, pin: %d}\n",
							// pin->block, pin->block_port, pin->block_pin);

						if (pin->block == the_pin.block
							&& ipin == the_pin.block_pin
							&& iport == the_pin.block_port) {
							// puts("found match");
							// we've found our pin
							*pin_index = i;
						}
						
						i = i + 1;
					}
				}
				i = i + 1;
			}
		}
		port = port->next;
	}
	*total_pins = i;
	// if (*pin_index < 0) {
	// 	puts("didn't find!!!!");
	// }
	// puts("--- NEXT! ---");
}

/**
 * Draws ONE logical connection from src_pin in src_lblk to sink_pin in sink_lblk.
 * The *_abs_bbox parameters are for mild optmization, as the absolute bbox can be calculated
 * more effeciently else where.
 */
void draw_one_logical_connection(
	const t_net_pin& src_pin,  const t_logical_block& src_lblk, const t_bound_box& src_abs_bbox,
	const t_net_pin& sink_pin, const t_logical_block& sink_lblk, const t_bound_box& sink_abs_bbox) {

	const float FRACTION_USABLE_WIDTH = 0.3;

	float src_width =  src_abs_bbox.get_width();
	float sink_width = sink_abs_bbox.get_width();

	float src_usable_width =  src_width  * FRACTION_USABLE_WIDTH;
	float sink_usable_width = sink_width * FRACTION_USABLE_WIDTH;

	float src_x_offset = src_abs_bbox.left() + src_width * (1 - FRACTION_USABLE_WIDTH)/2;
	float sink_x_offset = sink_abs_bbox.left() + sink_width * (1 - FRACTION_USABLE_WIDTH)/2;

	int src_pin_index, sink_pin_index, src_pin_total, sink_pin_total;

	find_pin_index_at_model_scope(src_pin,  src_lblk, false,  &src_pin_index, &src_pin_total );
	find_pin_index_at_model_scope(sink_pin, sink_lblk, true, &sink_pin_index, &sink_pin_total);

	const t_point src_point =  {
		src_x_offset + src_usable_width * src_pin_index / ((float)src_pin_total),
		src_abs_bbox.get_ycenter()
	};
	const t_point sink_point = {
		sink_x_offset + sink_usable_width * sink_pin_index / ((float)sink_pin_total),
		sink_abs_bbox.get_ycenter()
	};

	// draw a link connecting the pins.
	drawline(src_point.x, src_point.y,
		sink_point.x, sink_point.y);

	if (src_lblk.clb_index == sink_lblk.clb_index) {
		// if they are in the same clb, put one arrow in the center
		float center_x = (src_point.x + sink_point.x) / 2;
		float center_y = (src_point.y + sink_point.y) / 2;

		draw_triangle_along_line(
			center_x, center_y,
			src_point.x, sink_point.x,
			src_point.y, sink_point.y
		);
	} else {
		// if they are not, put 2 near each end
		draw_triangle_along_line(
			3,
			src_point.x, sink_point.x,
			src_point.y, sink_point.y
		);
		draw_triangle_along_line(
			-3,
			src_point.x, sink_point.x,
			src_point.y, sink_point.y
		);
	}

}

/* This function checks whether a top-level clb has been highlighted. It does 
 * so by checking whether the color in this block is default color.
 */
static bool is_top_lvl_block_highlighted(const t_block& clb) {
	t_draw_state *draw_state;
	ptrdiff_t blk_id = &clb - block;

	/* Call accessor function to retrieve global variables. */
	draw_state = get_draw_state_vars();

	if (clb.type->index < 3) {
		if (draw_state->block_color[blk_id] == LIGHTGREY)
			return false;
	} else if (clb.type->index < 3 + MAX_BLOCK_COLOURS) {
		if (draw_state->block_color[blk_id] == (BISQUE + MAX_BLOCK_COLOURS 
												+ clb.type->index - 3))
			return false;
	} else {
		if (draw_state->block_color[blk_id] == (BISQUE + 2 * MAX_BLOCK_COLOURS - 1))
			return false;
	}

	return true;
}

int highlight_sub_block(float abs_x, float abs_y) {
	t_draw_state* draw_state = get_draw_state_vars();
	t_draw_coords* draw_coords = get_draw_coords_vars();

	int max_depth = draw_state->show_blk_internal;
	// determine block, pass it in
	t_block* clb = NULL;
	t_bound_box clb_bbox(0,0,0,0);
	int i, j, k;

	// determine grid x
	for (i = 0; i <= nx + 1; ++i) {
		if (draw_coords->tile_x[i] > abs_x) {
			break;
		}
	}
	--i;

	// determine grid y
	for(j = 0; j <= ny + 1; ++j) {
		if (draw_coords->tile_y[j] > abs_y) {
			break;
		}
	}
	--j;

	// determine sub_block
	t_grid_tile* grid_tile = &grid[i][j];
	for (k = 0; k < grid_tile->type->capacity; ++k) {
		int clb_index = grid_tile->blocks[k];
		if (clb_index >= 0) {
			clb_bbox = draw_coords->
				get_absolute_pb_bbox(block[clb_index], block[clb_index].pb->pb_graph_node);
			if (clb_bbox.intersects(abs_x, abs_y)) {
				clb = &block[clb_index];
				break;
			}
		}
	}

	if (clb == NULL) {
		return 1;
	}

	t_point point_in_clb = t_point(abs_x, abs_y) - clb_bbox.bottom_left();

	t_pb* new_selected_sub_block = 
		highlight_sub_block_helper(clb, clb->pb, point_in_clb, max_depth);
	if (new_selected_sub_block == NULL) {
		get_selected_sub_block_info().clear();
		return 1;
	} else {
		get_selected_sub_block_info().set(new_selected_sub_block, clb);
		return 0;
	}
}

/**
 * The recursive part of highlight_sub_block. finds which pb is under
 * the given location. local_pt is relative to the given pb, and pb should
 * be in clb.
 */
t_pb* highlight_sub_block_helper(
	t_block* clb, t_pb* pb, t_point local_pt, int max_depth)
{
	
	t_draw_coords *draw_coords = get_draw_coords_vars();
	t_pb_type* pb_type = pb->pb_graph_node->pb_type;

	// check to see if we are past the displayed level,
	// if pb has children,
	// and if pb is dud
	if (pb_type->depth + 1 > max_depth
	 || pb->child_pbs == NULL
	 || pb_type->num_modes == 0) {
		return NULL;
	}

	int num_child_types = pb->get_num_child_types();

	// Iterate through each type of child pb, and each pb of that type.
	for (int i = 0; i < num_child_types; ++i) {
		int num_children_of_type = pb->get_num_children_of_type(i);

		for (int j = 0; j < num_children_of_type; ++j) {

			if (pb->child_pbs[i] == NULL) {
				continue;
			}

			t_pb* child_pb = &pb->child_pbs[i][j];
			t_pb_graph_node* pb_child_node = child_pb->pb_graph_node;

			// get the bbox for this child
			const t_bound_box& bbox = draw_coords->get_pb_bbox(*clb, *pb_child_node);

			// If child block is being used, check if it intersects
			if (child_pb->name != NULL && bbox.intersects(local_pt)) {

				// check farther down the graph, see if we can find
				// something more specific.
				t_pb* subtree_result =
					highlight_sub_block_helper(
						clb, child_pb, local_pt - bbox.bottom_left(), max_depth);
				if (subtree_result != NULL) {
					// we found something more specific.
					return subtree_result;
				} else {
					// couldn't find something more specific, return parent
					return child_pb;
				}
			}
		}
	}
	return NULL;
}

float calc_text_xbound(const t_bound_box& center_of, char* const text) {
	return calc_text_xbound(center_of.left(), center_of.bottom(), center_of.right(), center_of.top(), text);
}

/**
 * Returns a hopefully useful xbound with this in mind:
 *     limit the text display length to just outside of the bounding box,
 *     and limit it by the the height of the bounding box, with an adjustment
 *     for the length of the text, so effectively by the height of the text.
 *     note that this is calibrated for a 16pt font
 */
float calc_text_xbound(float start_x, float start_y, float end_x, float end_y, char* const text) {
	return min((end_x - start_x) * 1.1f, (end_y - start_y) * strlen(text) * 0.5f);
}

/*
 * Begin definition of t_selected_sub_block_info functions.
 */

void t_selected_sub_block_info::set(t_pb* new_selected_sub_block, t_block* new_containing_block) {
	selected_pb = new_selected_sub_block;
	selected_pb_gnode = (selected_pb == NULL) ? NULL : selected_pb->pb_graph_node;
	containing_block = new_containing_block;
	sinks.clear();
	sources.clear();

	// printf("selecting in clb #%ld\n", new_containing_block - block);

	if (has_selection()) {
		for (int i = 0; i < num_logical_blocks; ++i) {
			const t_logical_block* lblk = &logical_block[i];
			// find the logical block that corrisponds to this pb.
			if (&block[lblk->clb_index] == get_containing_block()
				&& lblk->pb->pb_graph_node == get_selected_pb_gnode()) {

				t_model* model = lblk->model;

				t_model_ports* model_ports = model->inputs;
				// iterate over the input ports
				while(model_ports != NULL) {
					if(model_ports->is_clock == FALSE) {
						int iport = model_ports->index;
						// iterate over the pins on that port
						for (int ipin = 0; ipin < model_ports->size; ipin++) {
							// get the net that connects here.
							int inet = lblk->input_nets[iport][ipin];
							if(inet != OPEN) {
								t_vnet& net = g_atoms_nlist.net.at(inet);

								// put the driver in the sources list
								t_net_pin& pin = net.pins.at(0);
								t_logical_block* src_lblk = &logical_block[pin.block];

								sources.insert(std::make_pair(
									src_lblk->pb->pb_graph_node, &block[src_lblk->clb_index])
								);
							}
						}
					}
					model_ports = model_ports->next;
				}

				model_ports = model->outputs;
				while(model_ports != NULL) {
					int iport = model_ports->index;
					for (int ipin = 0; ipin < model_ports->size; ipin++) {
						int inet = lblk->output_nets[iport][ipin];
						if(inet != OPEN) {
							t_vnet& net = g_atoms_nlist.net.at(inet);

							// put the all sinks in the sink list
							for (auto pin = net.pins.begin() + 1; pin != net.pins.end(); ++pin) {
								t_logical_block* sink_lblk = &logical_block[pin->block];

								sinks.insert(std::make_pair(
									sink_lblk->pb->pb_graph_node, &block[sink_lblk->clb_index])
								);
							}
						}
					}
					model_ports = model_ports->next;
				}

				break;
			}
		}
	}
}

t_selected_sub_block_info::t_selected_sub_block_info() {
	clear();
}

void t_selected_sub_block_info::clear() {
	set(NULL, NULL);
}

t_pb* t_selected_sub_block_info::get_selected_pb() const { return selected_pb; }

t_pb_graph_node* t_selected_sub_block_info::get_selected_pb_gnode() const { return selected_pb_gnode; }

t_block* t_selected_sub_block_info::get_containing_block() const { return containing_block; }

bool t_selected_sub_block_info::has_selection() const {
	return get_selected_pb_gnode() != NULL && get_containing_block() != NULL; 
}

bool t_selected_sub_block_info::is_selected(const t_pb_graph_node* test, const t_block* test_block) const {
	return get_selected_pb_gnode() == test && get_containing_block() == test_block;
}

bool t_selected_sub_block_info::is_sink_of_selected(const t_pb_graph_node* test, const t_block* test_block) const {
	return sinks.find(std::make_pair(test,test_block)) != sinks.end();
}

bool t_selected_sub_block_info::is_source_of_selected(const t_pb_graph_node* test, const t_block* test_block) const {
	return sources.find(std::make_pair(test,test_block)) != sources.end();
}

t_selected_sub_block_info& get_selected_sub_block_info() {
	// used to keep track of the selected sub-block.
	static t_selected_sub_block_info selected_sub_block_info;

	return selected_sub_block_info;
}
