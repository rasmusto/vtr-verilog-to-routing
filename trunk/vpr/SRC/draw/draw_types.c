#include "draw_global.h"
#include "draw_types.h"
#include "globals.h"
#include "vpr_utils.h"

/*******************************************
 * begin t_draw_state function definitions *
 *******************************************/

t_draw_state::t_draw_state() :
	pic_on_screen(NO_PICTURE),
	show_nets(DRAW_NO_NETS),
	show_congestion(DRAW_NO_CONGEST),
	draw_rr_toggle(DRAW_NO_RR),
	max_sub_blk_lvl(0),
	show_blk_internal(0),
	show_graphics(FALSE),
	gr_automode(0),
	draw_route_type(GLOBAL),
	net_color(NULL),
	block_color(NULL),
	draw_rr_node(NULL) { 

}

void t_draw_state::reset_nets_congestion_and_rr() {
	show_nets = DRAW_NO_NETS;
	draw_rr_toggle = DRAW_NO_RR;
	show_congestion = DRAW_NO_CONGEST;
}

/******************************************
 * begin t_draw_bbox function definitions *
 ******************************************/

const float& t_draw_bbox::left() const { return bottom_left().x; }
const float& t_draw_bbox::right() const { return top_right().x; }
const float& t_draw_bbox::bottom() const { return bottom_left().y; }
const float& t_draw_bbox::top() const { return top_right().y; }
const t_point& t_draw_bbox::bottom_left() const { return bottomleft; }
const t_point& t_draw_bbox::top_right() const { return topright; }
float& t_draw_bbox::left() { return bottom_left().x; }
float& t_draw_bbox::right() { return top_right().x; }
float& t_draw_bbox::bottom() { return bottom_left().y; }
float& t_draw_bbox::top() { return top_right().y; }
t_point& t_draw_bbox::bottom_left() { return bottomleft; }
t_point& t_draw_bbox::top_right() { return topright; }

float t_draw_bbox::get_xcenter() const {
	return (right() + left()) / 2;
}

float t_draw_bbox::get_ycenter() const {
	return (top() + bottom()) / 2;
}

float t_draw_bbox::get_width() const {
	return abs(right() - left());
}
float t_draw_bbox::get_height() const {
	return abs(top() - bottom());
}

void t_draw_bbox::offset(const t_point& relative_to) {
	this->bottomleft += relative_to;
	this->topright += relative_to;
}

/**************************************************
 * begin t_draw_pb_type_info function definitions *
 **************************************************/

t_draw_bbox& t_draw_pb_type_info::get_pb_bbox(const t_pb_graph_node& pb_graph_node) {
	const int pb_gnode_id = get_unique_pb_graph_node_id(&pb_graph_node);
	return subblk_array.at(pb_gnode_id);
}

/********************************************
 * begin t_draw_corrds function definitions *
 ********************************************/

t_draw_bbox& t_draw_coords::get_pb_bbox(int clb_index, const t_pb_graph_node& pb_gnode) {
	return get_pb_bbox(block[clb_index], pb_gnode);
}

t_draw_bbox& t_draw_coords::get_pb_bbox(const t_block& clb, const t_pb_graph_node& pb_gnode) {
	const int clb_type_id = clb.type->index;

	return this->blk_info.at(clb_type_id).get_pb_bbox(pb_gnode);
}

t_draw_bbox t_draw_coords::get_absolute_pb_bbox(const t_block& clb, t_pb_graph_node* pb_gnode) {
	t_draw_bbox result = this->get_pb_bbox(clb,*pb_gnode);
	
	while (pb_gnode->parent_pb_graph_node != NULL) {
		t_draw_bbox& parents_bbox = this->get_pb_bbox(clb, *pb_gnode->parent_pb_graph_node);
		result.offset(parents_bbox.bottom_left());
		// result.left() += parents_bbox.left();
		// result.right() += parents_bbox.left();
		// result.top() += parents_bbox.bottom();
		// result.bottom() += parents_bbox.bottom();
		pb_gnode = pb_gnode->parent_pb_graph_node;
	}

	t_point clb_coords(this->tile_x[clb.x], this->tile_y[clb.y]);	

	result.offset(clb_coords);

	return result;
}

t_draw_bbox t_draw_coords::get_absolute_pb_bbox(int clb_index, t_pb_graph_node* pb_gnode) {
	return get_absolute_pb_bbox(block[clb_index], pb_gnode);
}
