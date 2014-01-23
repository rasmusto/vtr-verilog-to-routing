/*
DESCRIPTION
The functions in this file are used to help build the switchblocks in rr_graph.c:build_rr_graph
(partially) based on the information in physical_types.h:t_switchblock_inf parsed from the switchblock section
of the XML architecture file.
*/

/* TODO TODO TODO!! in t_seg_details he referes to 'group' as a collection of L tracks that are consecutive and all 
			all with different start points. I'm referring to 'group' as the collection of tracks 
			in a channel going in the same direction and having the same start point (going by literature,
			this seems to be the more appropriate definition of group) */

#include <cstring>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "build_switchblocks.h"
#include "physical_types.h"
#include "parse_switchblocks.h"
#include "util.h"

using namespace std;

/**** Defines ****/
#define REF_X 1
#define REF_Y 1

/**** Classes ****/
/* contains info about a track type */
class Track_Info{
public:
	int length;		/* the length of this type of track */
	int num_tracks;		/* total number of tracks */
	int start;		/* the track index at which this type starts */
	
	void set(int len, int tracks, int st){
		length = len; num_tracks = tracks; start = st;
	}
	Track_Info(){
		length = num_tracks = start = 0;
	}
	Track_Info(int len, int tracks, int st){
		length = len; num_tracks = tracks; start = st;
	}
};


/**** Typedefs ****/
/* Used for translating the name of a track type to the number of tracks belonging to this type */
typedef map< string, Track_Info > t_track_type_sizes;

/* wire type -> wirepoint -> wirepoint start index map. Used for determining which tracks correspond to
   a specific wirepoint at coordinate (x,y) based on some reference coordinate the info for which is 
   contained in this map */
typedef map< string, map< pair< e_side, int >, int > > t_wirepoint_start_map;


/**** Function Declarations ****/
/* determines the track index at which each possible wire type->wirepoint start in some reference switchblock (specified
	by the coordinates ref_x, ref_y) */
static void determine_wirepoint_starts(INP e_directionality directionality,
		INP int nx, INP int ny, INP t_chan_details *chan_details_x, 
		INP t_chan_details *chan_details_y,INP t_track_type_sizes *track_type_sizes, 
		INP int nodes_per_chan, OUTP t_wirepoint_start_map *wirepoint_starts);

/* over all connected wire types (i,j), compute the maximum least common multiple of their track lengths, 
   ie max(LCM(L_i, L_J)) */
static int get_max_lcm( INP vector<t_switchblock_inf> *switchblocks, INP t_track_type_sizes *track_type_sizes);

/* Counts the number of tracks in each track type in the specified channel */
static void count_track_type_sizes(INP t_seg_details *channel, INP int nodes_per_chan, 
			INOUTP t_track_type_sizes *track_type_sizes);

/* computes the switchblocks at the corners of the FPGA */
static void compute_corner_switchblocks(INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
		INP vector<t_switchblock_inf> *switchblocks, INP int nx, INP int ny, INP int nodes_per_chan,
		INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality,
		INP t_wirepoint_start_map *wirepoint_starts, INOUTP t_sb_connection_map *sb_conns);

/* compute all the switchblocks around the perimeter of the FPGA for the given switchblock and wireconn */
static void compute_perimeter_switchblocks(INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
		INP vector<t_switchblock_inf> *switchblocks, INP int nx, INP int ny, INP int nodes_per_chan,
		INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality,
		INP t_wirepoint_start_map *wirepoint_starts, INOUTP t_sb_connection_map *sb_conns);

/* computes a horizontal line of switchblocks of size sb_line_size (or of nx-2, whichever is smaller), starting 
   at coordinate (1,1) */
static void compute_switchblock_line(INP int sb_line_size, INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
		INP vector<t_switchblock_inf> *switchblocks, INP int nx, INP int ny, INP int nodes_per_chan, 
		INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality,
		INP t_wirepoint_start_map *wirepoint_starts, INOUTP t_sb_connection_map *sb_line);

/* stamp out a line of horizontal switchblocks starting at coordinates (ref_x, ref_y) and
   continuing on for sb_line_size */
static void stampout_switchblock_line( INP int sb_line_size,  
		INP t_wirepoint_start_map *wirepoint_starts, INP int nodes_per_chan,
		INP int nx, INP int ny, INP t_track_type_sizes *track_type_sizes, 
		INP e_directionality directionality, INP t_sb_connection_map *sb_line, 
		INOUTP t_sb_connection_map *sb_conns );

/* Compute the track that the track at (x, y, from_side, to_side, from_track) should connect to.
   sb_conns is updated with the result */
static void compute_track_connections(INP int x_coord, INP int y_coord, INP enum e_side from_side,
			INP enum e_side to_side, INP int from_track, INP t_chan_details * chan_details_x,
			INP t_chan_details * chan_details_y, INP t_switchblock_inf *sb,
			INP int nx, INP int ny, INP int nodes_per_chan, INP t_wirepoint_start_map *wirepoint_starts, 
			INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality, 
			INOUTP t_sb_connection_map *sb_conns);

/* returns the track indices belonging to the types in track_type_vec and wirepoint in point at the given 
   coordinates/side */ 
static void get_wirepoint_tracks( INP e_directionality directionality, INP int nx, INP int ny, INP int x, INP int y, 
		INP e_side side, INP vector<string> *track_type_vec, INP vector<int> *points, INP t_track_type_sizes *track_type_sizes,
		INP t_wirepoint_start_map *wirepoint_starts, INP bool is_dest, INOUTP vector<int> *tracks);

/* wirepoint_starts contains the track indices of each wire type and wire point at a reference switchblock. 
   the wirepoint indices can be translated to another coordinate based on the distance between this and the
   reference coordinates. Here the index of the first track at the specified wirepoint/side is returned
   based on the specified distance between the desired and reference switchblocks. Returns -1 if 
   specified type doesn't have the specified wirepoint */ 
static int get_adjusted_wirepoint_start(INP e_directionality directionality, INP int wirepoint, INP e_side side, 
		INP string track_type, INP int num_type_tracks, INP int type_track_length, INP int first_type_track, 
		INP int last_type_track, INP int distance, INP int dir_adjust, INP bool is_dest, 
		INP t_wirepoint_start_map *wirepoint_starts);

static t_rr_type index_into_correct_chan(INP int tile_x, INP int tile_y, INP enum e_side side, 
			INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
			OUTP int *chan_x, OUTP int *chan_y, 
			OUTP t_chan_details **chan_details);

/* checks whether the specified coordinates are out of bounds */
static bool coords_out_of_bounds(INP int nx, INP int ny, INP int x_coord, INP int y_coord, 
			e_rr_type chan_type);

/* returns seg coordinate based on a channel's x/y coordinates */
static int get_seg_coordinate(INP t_rr_type chan_type, INP int x_coord, INP int y_coord);

/* returns the direction in which a destination track at the specified
   switchblock side should be headed */
static e_direction get_dest_direction(e_side to_side, e_directionality directionality);

/* returns the direction in which a source track at the specified
   switchblock side should be headed. */
static e_direction get_src_direction(e_side from_side, e_directionality directionality);

/* returns the group number of the specified track */
static int get_track_group(INP int nx, INP int ny, INP e_rr_type chan_type,
			 INP t_seg_details &track_details, int seg_coord);

/* Finds the index of the first track in the channel specified by 'track_details'
   to have the type name specified by 'type' */
static int find_start_of_track_type(INP t_seg_details *track_details, INP const char *type, 
			INP int nodes_per_chan, INP t_track_type_sizes *track_type_sizes);

/* returns the track index in the channel specified by 'track_details' that corresponds to the 
   first occurence of the specified track group (belonging to type referred to by track_type_start, 
   track_group, and track_direction) */
static int find_start_of_track_group(INP int nx, INP int ny, INP t_rr_type chan_type, 
			INP t_seg_details *track_details, INP const char *type, 
			INP int track_group, INP int track_seg_coord, INP e_direction track_direction, 
			INP int track_type_start, INP int nodes_per_chan, INP t_track_type_sizes *track_type_sizes);
	
/* fills the 'tracks_in_group' vector with the indices of the tracks in the 'track_details' channel
   which have the specified 'track_type', 'track_group', and are going in 'track_direction' */
static void get_group_tracks(INP int nx, INP int ny, INP t_seg_details *track_details, INP t_rr_type chan_type,
			INP const char *track_type, INP int track_group, INP int track_seg_coord, 
			INP e_direction track_direction, INP int nodes_per_chan, INP int type_start, 
			INP t_track_type_sizes *track_type_sizes, INOUTP vector<int> *tracks);

/* Returns the wirepoint of the track specified by track_details at a segment coordinate
   of seg_coord, and connection to the sb_side of the switchblock */
static int get_wirepoint_of_track(INP int nx, INP int ny, INP e_rr_type chan_type,
		INP t_seg_details &track_details, INP int seg_coord, INP e_side sb_side);

static void get_wirepoint_tracks(INP int nx, INP int ny, INP t_seg_details *track_details, INP t_rr_type chan_type,
			INP const char *track_type, INP vector<int> *track_wirepoint, INP int track_seg_coord, 
			INP e_direction track_direction, INP int nodes_per_chan, INP t_track_type_sizes *track_type_sizes, 
			INP e_side sb_side, INOUTP vector<int> *tracks);

/* returns true if the coordinates x/y do not correspond to the location specified by 'location' */
static bool sb_not_here(INP int nx, INP int ny, INP int x, INP int y, INP e_sb_location location);

/* checks if the specified coordinates represent a corner of the FPGA */
static bool is_corner(INP int nx, INP int ny, INP int x, INP int y);

/* checks if the specified coordinates correspond to one of the perimeter switchblocks */
static bool is_perimeter(INP int nx, INP int ny, INP int x, INP int y);

/* checks if the specified coordinates correspond to the core of the FPGA (i.e. not perimeter) */
static bool is_core(INP int nx, INP int ny, INP int x, INP int y);

/* adjusts a negative destination track calculated from a permutation formula */
static int adjust_formula_result(INP int dest_track, INP int dest_W);


/**** Function Definitions ****/


/* allocate and build the switchblock permutation map */
t_sb_connection_map * alloc_and_load_switchblock_permutations( INP t_chan_details * chan_details_x, 
				INP t_chan_details * chan_details_y, INP int nx, INP int ny, 
				INP vector<t_switchblock_inf> switchblocks, 
				INP int nodes_per_chan, INP e_directionality directionality){


	/* see t_wirepoint_start_map description */
	t_wirepoint_start_map wirepoint_starts;

	/* the switchblock permutations pointer */
	t_sb_connection_map *sb_conns = new t_sb_connection_map;

	/* We assume that x & y channels have the same ratios of track types. i.e., looking at a single 
	   channel is representative of all channels in the FPGA -- as of 3/9/2013 this is true in VPR */
	t_track_type_sizes track_type_sizes;
	count_track_type_sizes(chan_details_x[0][0], nodes_per_chan, &track_type_sizes);
	
	/* determine at which track index each wirepoint starts at a reference switchblock of 
	   coordinate (1,1) */
	determine_wirepoint_starts(directionality, nx, ny, chan_details_x,
			chan_details_y, &track_type_sizes, nodes_per_chan, &wirepoint_starts);


#if 1
	/* figure out max(lcm(L_i, L_j)) for all wire lengths belonging to wire types i & j */
	int max_lcm = get_max_lcm(&switchblocks, &track_type_sizes);
	t_sb_connection_map sb_line;

	/* compute the perimeter switchblocks. unfortunately we can't just compute corners and stamp out the rest because
	   for a unidirectional architecture corners AND perimeter switchblocks require special treatment */
	compute_perimeter_switchblocks( chan_details_x, chan_details_y, &switchblocks,
			nx, ny, nodes_per_chan, &track_type_sizes, directionality, &wirepoint_starts, sb_conns);

	/* compute the switchblock line */
	compute_switchblock_line( max_lcm, chan_details_x, chan_details_y, &switchblocks,
			nx, ny, nodes_per_chan, &track_type_sizes, directionality, &wirepoint_starts, &sb_line );

	/* stamp-out the switchblock line throughout the rest of the FPGA */
	stampout_switchblock_line( max_lcm, &wirepoint_starts, nodes_per_chan,
			nx, ny, &track_type_sizes, directionality, &sb_line, sb_conns );

#else
	/* iterate over all the switchblocks specified in the architecture */
	for (int i_sb = 0; i_sb < (int)switchblocks.size(); i_sb++){
		t_switchblock_inf sb = switchblocks[i_sb];

		/* verify that switchblock type matches specified directionality -- currently we have to stay consistent */
		if (directionality != sb.directionality){
			vpr_printf(TIO_MESSAGE_ERROR, "alloc_and_load_switchblock_connections: Switchblock %s does not match directionality of architecture\n", sb.name.c_str());
			exit(1);
		}
		/* Iterate over the x,y coordinates spanning the FPGA. Currently, the FPGA size is set as
		   (0..nx+1) by (0..ny+1), so we iterate over 0..nx and 0..ny. */
		for (int x_coord = 0; x_coord <= nx+1; x_coord++){
			for (int y_coord = 0; y_coord <= ny+1; y_coord++){
				if (sb_not_here(nx, ny, x_coord, y_coord, sb.location)){
					continue;
				}
				/* Iterate over each track in channel */
				for (int from_track = 0; from_track < nodes_per_chan; from_track++){

					/* now we iterate over all the potential side1->side2 connections */
					for ( e_side from_side = (e_side) 0; from_side < 4; from_side = (e_side)(from_side + 1) ){
						for ( e_side to_side = (e_side) 0; to_side < 4; to_side = (e_side)(to_side + 1) ){
							
							/* Fill appropriate entry of the sb_conns map with vector specifying the tracks 
							   the current track will connect to */
							compute_track_connections(x_coord, y_coord, from_side, to_side, from_track,
									chan_details_x, chan_details_y, &sb, nx, ny, nodes_per_chan,
									&wirepoint_starts, &track_type_sizes, directionality, sb_conns);
							
						}
					}
				}
			}
		}
	}
#endif
	return sb_conns;
}

/* calls delete on the switchblock permutations pointer and sets it to 0 */
void free_switchblock_permutations(INOUTP t_sb_connection_map *sb_conns){
	delete sb_conns;
	sb_conns = NULL;
	return;
}

/* over all connected wire types (i,j), compute the maximum least common multiple of their track lengths, 
   ie max(LCM(L_i, L_J)) */
static int get_max_lcm( INP vector<t_switchblock_inf> *switchblocks, INP t_track_type_sizes *track_type_sizes){
	int max_lcm = -1;
	int num_sb = (int)switchblocks->size();

	/* over each switchblock */
	for (int isb = 0; isb < num_sb; isb++){
		t_switchblock_inf *sb = &(switchblocks->at(isb));
		int num_wireconns = (int)sb->wireconns.size();
		/* over each wireconn */
		for (int iwc = 0; iwc < num_wireconns; iwc++){
			t_wireconn_inf *wc = &(sb->wireconns.at(iwc));
			int num_from_types = (int)wc->from_type.size();
			int num_to_types = (int)wc->to_type.size();
			/* over each from type */
			for (int ifrom = 0; ifrom < num_from_types; ifrom++){
				/* over each to type */
				for (int ito = 0; ito < num_to_types; ito++){
					int length1 = track_type_sizes->at(wc->from_type.at(ifrom)).length;
					int length2 = track_type_sizes->at(wc->to_type.at(ito)).length;
					int current_lcm = lcm(length1, length2);
					if (current_lcm > max_lcm){
						max_lcm = current_lcm;
					}
				}
			}
		}
	}
	
	return max_lcm;
}

/* computes a horizontal line of switchblocks of size sb_line_size (or of nx-2, whichever is smaller), starting 
   at coordinate (1,1) */
static void compute_switchblock_line(INP int sb_line_size, INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
		INP vector<t_switchblock_inf> *switchblocks, INP int nx, INP int ny, INP int nodes_per_chan, 
		INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality,
		INP t_wirepoint_start_map *wirepoint_starts, INOUTP t_sb_connection_map *sb_line){

	int y = 1;
	for (int isb = 0; isb < (int)switchblocks->size(); isb++){
		t_switchblock_inf *sb = &(switchblocks->at(isb));
		for (int x = 1; x < 1 + sb_line_size; x++){
			if (sb_not_here(nx, ny, x, y, sb->location)){
				continue;
			}
			/* Iterate over each track in channel */
			for (int from_track = 0; from_track < nodes_per_chan; from_track++){
				/* now we iterate over all the potential side1->side2 connections */
				for ( e_side from_side = (e_side) 0; from_side < 4; from_side = (e_side)(from_side + 1) ){
					for ( e_side to_side = (e_side) 0; to_side < 4; to_side = (e_side)(to_side + 1) ){
						
						/* Fill appropriate entry of the sb_conns map with vector specifying the tracks 
						   the current track will connect to */
						compute_track_connections(x, y, from_side, to_side, from_track,
								chan_details_x, chan_details_y, sb, nx, ny, nodes_per_chan,
								wirepoint_starts, track_type_sizes, directionality, sb_line);
					}
				}
			}
		}
	}
}

/* returns true if the coordinates x/y do not correspond to the location specified by 'location' */
static bool sb_not_here(INP int nx, INP int ny, INP int x, INP int y, INP e_sb_location location){
	bool sb_not_here = true;

	switch( location ){
		case E_EVERYWHERE:
			sb_not_here = false;
			break;
		case E_PERIMETER:
			if (is_perimeter(nx, ny, x, y)){
				sb_not_here = false;
			}
			break;
		case E_CORNER:
			if (is_corner(nx, ny, x, y)){
				sb_not_here = false;
			}
			break;
		case E_CORE:
			if (is_core(nx, ny, x, y)){
				sb_not_here = false;
			}
			break;
		case E_FRINGE:
			if (is_perimeter(nx, ny, x, y) && !is_corner(nx, ny, x, y)){
				sb_not_here = false;
			}
			break;
		default:
			vpr_printf(TIO_MESSAGE_ERROR, "sb_not_here: unrecognized location enum: %d\n", location);
			break;
	}
	return sb_not_here;
}

/* checks if the specified coordinates represent a corner of the FPGA */
static bool is_corner(INP int nx, INP int ny, INP int x, INP int y){
	bool is_corner = false;
	if ((x == 0 && y == 0) ||
	    (x == 0 && y == ny) ||
	    (x == nx && y == 0) ||
	    (x == nx && y == ny)){
		is_corner = true;
	}
	return is_corner;
}

/* checks if the specified coordinates correspond to one of the perimeter switchblocks */
static bool is_perimeter(INP int nx, INP int ny, INP int x, INP int y){
	bool is_perimeter = false;
	if (x == 0 || x == nx ||
	    y == 0 || y == ny){
		is_perimeter = true;
	}
	return is_perimeter;
}

/* checks if the specified coordinates correspond to the core of the FPGA (i.e. not perimeter) */
static bool is_core(INP int nx, INP int ny, INP int x, INP int y){
	bool is_core = !is_perimeter(nx, ny, x, y);
	return is_core;
}
	
/* stamp out a line of horizontal switchblocks starting at coordinates (ref_x, ref_y) and
   continuing on for sb_line_size */
static void stampout_switchblock_line( INP int sb_line_size,  
		INP t_wirepoint_start_map *wirepoint_starts, INP int nodes_per_chan,
		INP int nx, INP int ny, INP t_track_type_sizes *track_type_sizes, 
		INP e_directionality directionality, INP t_sb_connection_map *sb_line, 
		INOUTP t_sb_connection_map *sb_conns ){

	/* over all x coordinates that may need stamping out*/
	for (int x = 1; x < nx; x++){
		/* over all y coordinates that may need stamping out */
		for (int y = 1; y < ny; y++){
			/* perimeter has been precomputed */
			if ( is_perimeter(nx, ny, x, y) ){
				continue;
			}
			/* over each source side */
			for (int from_side = 0; from_side < 4; from_side ++){
				/* over each destination side */
				for (int to_side = 0; to_side < 4; to_side ++){
					/* can't connect a side to itself */
					if (from_side == to_side){
						continue;
					}
					/* over each source track */
					for (int itrack = 0; itrack < nodes_per_chan; itrack++){
						/* get the total x+y distance of the current switchblock from the reference switchblock */
						int distance = (x - REF_X) + (y - REF_Y);
						if (distance < 0){
							distance = sb_line_size - ((-1*distance)%sb_line_size);
						}
						/* figure out the coordinates of the switchblock we want to copy */
						int copy_y = 1;
						int copy_x = 1 + (distance % sb_line_size);

						/* create the indices to key into the switchblock permutation map */
						Switchblock_Lookup my_key(x, y, from_side, to_side, itrack);
						Switchblock_Lookup copy_key(copy_x, copy_y, from_side, to_side, itrack);

						(*sb_conns)[my_key] = (*sb_line)[copy_key];
					}
				}
			}
		}
	}
}

/* computes the switchblocks at the corners of the FPGA */
static void compute_corner_switchblocks(INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
		INP vector<t_switchblock_inf> *switchblocks, INP int nx, INP int ny, INP int nodes_per_chan,
		INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality,
		INP t_wirepoint_start_map *wirepoint_starts, INOUTP t_sb_connection_map *sb_conns){
	int x, y;

	for (int isb = 0; isb < (int)switchblocks->size(); isb++){
		x = 0;
		y = 0;
		t_switchblock_inf *sb = &(switchblocks->at(isb));
		for (int i = 0; i < 2; i++){
			for (int j = 0; j < 2; j++){
				/* Iterate over each track in channel */
				for (int from_track = 0; from_track < nodes_per_chan; from_track++){
					/* now we iterate over all the potential side1->side2 connections */
					for ( e_side from_side = (e_side) 0; from_side < 4; from_side = (e_side)(from_side + 1) ){
						for ( e_side to_side = (e_side) 0; to_side < 4; to_side = (e_side)(to_side + 1) ){
							/* Fill appropriate entry of the sb_conns map with vector specifying the tracks 
							   the current track will connect to */
							compute_track_connections(x, y, from_side, to_side, from_track,
									chan_details_x, chan_details_y, sb, nx, ny, nodes_per_chan,
									wirepoint_starts, track_type_sizes, directionality, sb_conns);
						}
					}
				}
				x = nx;
			}
			y = ny;
			x = 0;
		}
	}
}

/* compute all the switchblocks around the perimeter of the FPGA for the given switchblock and wireconn */
static void compute_perimeter_switchblocks(INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
		INP vector<t_switchblock_inf> *switchblocks, INP int nx, INP int ny, INP int nodes_per_chan,
		INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality,
		INP t_wirepoint_start_map *wirepoint_starts, INOUTP t_sb_connection_map *sb_conns){
	int x, y;

	for (int isb = 0; isb < (int)switchblocks->size(); isb++){
		/* along left and right edge */
		x = 0;
		t_switchblock_inf *sb = &(switchblocks->at(isb));
		for (int i = 0; i < 2; i++){
			for (y = 0; y <= ny+1; y++){
				if (sb_not_here(nx, ny, x, y, sb->location)){
					continue;
				}
				/* Iterate over each track in channel */
				for (int from_track = 0; from_track < nodes_per_chan; from_track++){
					/* now we iterate over all the potential side1->side2 connections */
					for ( e_side from_side = (e_side) 0; from_side < 4; from_side = (e_side)(from_side + 1) ){
						for ( e_side to_side = (e_side) 0; to_side < 4; to_side = (e_side)(to_side + 1) ){

							/* Fill appropriate entry of the sb_conns map with vector specifying the tracks
							   the current track will connect to */
							compute_track_connections(x, y, from_side, to_side, from_track,
									chan_details_x, chan_details_y, sb, nx, ny, nodes_per_chan,
									wirepoint_starts, track_type_sizes, directionality, sb_conns);
						}
					}
				}
			}
			x = nx;
		}
	}

	for (int isb = 0; isb < (int)switchblocks->size(); isb++){
		/* along bottom and top edge */
		y = 0;
		t_switchblock_inf *sb = &(switchblocks->at(isb));
		for (int i = 0; i < 2; i++){
			for (x = 0; x <= nx+1; x++){
				if (sb_not_here(nx, ny, x, y, sb->location)){
					continue;
				}
				/* Iterate over each track in channel */
				for (int from_track = 0; from_track < nodes_per_chan; from_track++){
					/* now we iterate over all the potential side1->side2 connections */
					for ( e_side from_side = (e_side) 0; from_side < 4; from_side = (e_side)(from_side + 1) ){
						for ( e_side to_side = (e_side) 0; to_side < 4; to_side = (e_side)(to_side + 1) ){

							/* Fill appropriate entry of the sb_conns map with vector specifying the tracks
							   the current track will connect to */
							compute_track_connections(x, y, from_side, to_side, from_track,
									chan_details_x, chan_details_y, sb, nx, ny, nodes_per_chan,
									wirepoint_starts, track_type_sizes, directionality, sb_conns);
						}
					}
				}
			}
			y = ny;
		}
	}
}

/* determines the track index at which each possible wire type->wirepoint start in some reference switchblock (specified
	by the coordinates ref_x, ref_y) */
static void determine_wirepoint_starts(INP e_directionality directionality,
			INP int nx, INP int ny, INP t_chan_details *chan_details_x, 
		INP t_chan_details *chan_details_y, INP t_track_type_sizes *track_type_sizes, 
		INP int nodes_per_chan, OUTP t_wirepoint_start_map *wirepoint_starts){

	/* iterate over each track type */
	t_track_type_sizes::const_iterator it;
	for (it = track_type_sizes->begin(); it != track_type_sizes->end(); it++){
		string track_type = it->first;

		/* iterate over the four switchblock sides */
		for (int iside = 0; iside < 4; iside++){
			e_side sb_side = (e_side)iside;
		
			/* get the channel type (chanx/chany) and index into correct chan (chan_details_x/y) */
			t_chan_details *chan_details = NULL;
			int from_x, from_y;
			t_rr_type chan_type = index_into_correct_chan(REF_X, REF_Y, sb_side, chan_details_x,
						chan_details_y, &from_x, &from_y, &chan_details);

			/* get the seg coordinate at this (x,y) coordinate and channel type (chanx/chany) */
			int seg_coord = get_seg_coordinate(chan_type, from_x, from_y);
			
			/* get the track details at the reference switchblock */
			t_seg_details *track_details = chan_details[from_x][from_y];

			/* find the track index at which the current wire type starts */
			int type_start = find_start_of_track_type(track_details, track_type.c_str(), nodes_per_chan, track_type_sizes);
			
			/* get the wirepoint of the first track */
			int start_wirepoint = get_wirepoint_of_track(nx, ny, chan_type,
						track_details[type_start], seg_coord, sb_side);

			/* get the track length */
			int track_length = track_details[type_start].length;

			/* we want the track that can be used as the source of the connection, so in the case of a unidirectional
			   architecture, we might need to adjust type_start (this does not change its wirepoint) */
			if (UNI_DIRECTIONAL == directionality){
				e_direction type_start_direction = get_src_direction(sb_side, directionality);

				/* check direction of the type_start track */
				if (type_start_direction != track_details[type_start].direction){
					/* heading in the opposite direction. we need only to increment type_start to get
					   the track at the same wirepoint heading in the correct direction */
					type_start++;

					/* sanity check... */
					if (get_wirepoint_of_track(nx, ny, chan_type, track_details[type_start], seg_coord, sb_side)
					    != start_wirepoint){
						vpr_printf(TIO_MESSAGE_ERROR, "determine_wirepoint_starts(): expected wirepoint to remain the same, it did not\n");
						exit(1);
					}
				}
			}

			/* get the first index in the channel for each wirepoint. */
			int dir_adjust; 	/* a factor to adjust for different directionalities */
			if (BI_DIRECTIONAL == directionality){
				dir_adjust = 1;
			} else if (UNI_DIRECTIONAL == directionality){
				dir_adjust = 2;
			} else {
				vpr_printf(TIO_MESSAGE_ERROR, "determine_wirepoint_starts(): unknown directionality %d\n", directionality);
				exit(1);
			}
			int next_wirepoint = start_wirepoint;
			int next_wp_track = type_start;
			for (int iwp = 0; iwp < track_length; iwp++){
				pair< e_side, int > wp_index(sb_side, next_wirepoint);
				/* make sure this type/wirepoint/side combination doesn't exist yet */
				if ( map_key_exists(track_type, wirepoint_starts) ){
					if ( map_key_exists(wp_index, &(wirepoint_starts->at(track_type))) ){
						vpr_printf(TIO_MESSAGE_ERROR, "determine_wirepoint_starts(): wirepoint_starts at type %s, side %d, index %d already has entry\n",
							track_type.c_str(), wp_index.first, wp_index.second);
						exit(1);
					}
				}
				/* add index of next wirepoint to map */
				(*wirepoint_starts)[track_type][wp_index]= next_wp_track;

				/* increment wirepoint and wirepoint track */
				next_wirepoint = (next_wirepoint + (track_length-1)) % track_length;
				next_wp_track += dir_adjust;
			}
		}
	}
}

/* Counts the number of tracks in each track type in the specified channel */
static void count_track_type_sizes(INP t_seg_details *channel, INP int nodes_per_chan, 
			INOUTP t_track_type_sizes *track_type_sizes){
	string track_type;
	const char * new_type;
	int new_length, length;
	int new_start, start;
	int num_tracks = 0;
	Track_Info track_info;	

	track_type = channel[0].type_name_ptr;
	length = channel[0].length;
	start = 0;
	for (int itrack = 0; itrack < nodes_per_chan; itrack++){
		new_type = channel[itrack].type_name_ptr;
		new_length = channel[itrack].length;
		new_start = itrack;
		if (strcmp(new_type, track_type.c_str()) != 0){
			track_info.set(length, num_tracks, start);
			(*track_type_sizes)[track_type] = track_info;
			track_type = new_type;
			length = new_length;
			start = new_start;
			num_tracks = 0;
		}
		num_tracks++;
	}
	track_info.set(length, num_tracks, start);
	(*track_type_sizes)[track_type] = track_info;

	return;
} 

/* wirepoint_starts contains the track indices of each wire type and wire point at a reference switchblock. 
   the wirepoint indices can be translated to another coordinate based on the distance between this and the
   reference coordinates. Here the index of the first track at the specified wirepoint/side is returned
   based on the specified distance between the desired and reference switchblocks. Returns -1 if 
   specified type doesn't have the specified wirepoint */ 
static int get_adjusted_wirepoint_start(INP e_directionality directionality, INP int wirepoint, INP e_side side, 
		INP string track_type, INP int num_type_tracks, INP int type_track_length, INP int first_type_track, 
		INP int last_type_track, INP int distance, INP int dir_adjust, INP bool is_dest, 
		INP t_wirepoint_start_map *wirepoint_starts){
	int wirepoint_start = -1;

	if (distance < 0){
		distance = (dir_adjust * type_track_length) - (-1 * distance) % (dir_adjust * type_track_length);
	}
	
	pair<e_side, int> ind(side, wirepoint);
	if (map_key_exists(ind, &(wirepoint_starts->at(track_type)))){
		wirepoint_start = wirepoint_starts->at(track_type).at(ind);
		wirepoint_start = (wirepoint_start - first_type_track + dir_adjust*distance) 
				% (dir_adjust * type_track_length) + first_type_track;

		/* the wirepoint_starts that were computed at the reference coordinate were computed as potential source
		   tracks. so if we're looking for a track to serve as the destination of a connection in a unidir 
		   architecture, we need to further adjust the wirepoint start */
		if (UNI_DIRECTIONAL == directionality && is_dest){
			if (TOP == side || RIGHT == side){
				wirepoint_start--;
			} else {
				wirepoint_start++;
			}
		}
	} else {
		wirepoint_start = -1;
	}

	return wirepoint_start;
}


/* returns the track indices belonging to the types in track_type_vec and wirepoint in point at the given 
   coordinates/side */ 
static void get_wirepoint_tracks( INP e_directionality directionality, INP int nx, INP int ny, INP int x, INP int y, 
		INP e_side side, INP vector<string> *track_type_vec, INP vector<int> *points, INP t_track_type_sizes *track_type_sizes,
		INP t_wirepoint_start_map *wirepoint_starts, INP bool is_dest, INOUTP vector<int> *tracks){

	tracks->clear();

	int num_types = (int)track_type_vec->size();
	for (int itype = 0; itype < num_types; itype++){
		string track_type = track_type_vec->at(itype);

		/* get the number of tracks of given type */
		int num_type_tracks = track_type_sizes->at(track_type).num_tracks;
		/* get the length of this type of track */
		int type_track_length = track_type_sizes->at(track_type).length;
		/* get the last track belonging to this type */
		int first_type_track = track_type_sizes->at(track_type).start;
		int last_type_track = first_type_track + num_type_tracks - 1;

		/* distance between the reference switchblock that was used to calculate wirepoints
		   and the current coordinate */
		int distance = (x - REF_X) + (y - REF_Y);

		/* a factor to adjust for different directionalities */
		int dir_adjust;
		if (BI_DIRECTIONAL == directionality){
			dir_adjust = 1;
		} else if (UNI_DIRECTIONAL == directionality){
			dir_adjust = 2;
		} else {
			vpr_printf(TIO_MESSAGE_ERROR, "get_wirepoint_tracks(): unknown directionality %d\n", directionality);
			exit(1);
		}

		int increment;
		/* for each wirepoint */
		for (int iwp = 0; iwp < (int)points->size(); iwp++){

			int wirepoint = points->at(iwp);
			int wirepoint_start = get_adjusted_wirepoint_start(directionality, wirepoint, side, track_type, 
					num_type_tracks, type_track_length, first_type_track, last_type_track, distance, dir_adjust, 
					is_dest, wirepoint_starts);
				
			/* get_adjusted_wirepoint_start returns -1 if the given types doesn't have the specified wirepoint.
			   (this might happen if L=2 but wirepoint=4). We just ignore that */
			if (-1 == wirepoint_start){
				continue;
			}
	
			/* in a unidirectional arch, connections may only be made to wirepoint 0. in the corners of
			   the FPGA, and some sides of the perimeter tiles, all wires are of wirepoint 0 */
			if (UNI_DIRECTIONAL == directionality && 
			    is_dest && 
			    0 == wirepoint &&
			    ((TOP==side && y==0) ||
			     (RIGHT==side && x==0) ||
			     (LEFT==side && x==nx) ||
			     (BOTTOM==side && y==ny))){
				increment = dir_adjust;
				wirepoint_start = wirepoint_start - increment*floor((wirepoint_start - first_type_track) / increment);
			} else {
				increment = dir_adjust * type_track_length;
			}

			for (int itrack = wirepoint_start; itrack <= last_type_track; itrack += increment){
				tracks->push_back( itrack );
			}
		}
	}
	sort(tracks->begin(), tracks->end());
}


/* Compute the track that the track at (x, y, from_side, to_side, from_track) should connect to.
   sb_conns is updated with the result */
static void compute_track_connections(INP int x_coord, INP int y_coord, INP enum e_side from_side,
			INP enum e_side to_side, INP int from_track, INP t_chan_details * chan_details_x,
			INP t_chan_details * chan_details_y, INP t_switchblock_inf *sb,
			INP int nx, INP int ny, INP int nodes_per_chan, INP t_wirepoint_start_map *wirepoint_starts, 
			INP t_track_type_sizes *track_type_sizes, INP e_directionality directionality, 
			INOUTP t_sb_connection_map *sb_conns){

	t_chan_details *from_chan_details = NULL;	/* details for source channel */
	t_chan_details *to_chan_details = NULL ;	/* details for destination channel */
	int from_x, from_y;				/* index into source channel */
	int to_x, to_y;					/* index into destination channel */
	t_rr_type from_chan_type, to_chan_type;		/* the type of channel - i.e. CHANX or CHANY */
	e_direction src_direction, dest_direction;	/* the direction in which the source and destination tracks head */

	Connect_SB_Sides side_conn(from_side, to_side);		/* for indexing into this switchblock's permutation funcs */
	Switchblock_Lookup sb_conn(x_coord, y_coord, from_side, to_side, from_track);	/* for indexing into FPGA's switchblock map */

	/* can't connect a switchblock side to itself */
	if (from_side == to_side){
		return;
	}
	/* check that the permutation map has an entry for this side combination */
	t_permutation_map::const_iterator map_it = sb->permutation_map.find(side_conn);
	if (sb->permutation_map.end() == map_it){
		/* the specified switchblock does not have any permutation funcs for this side1->side2 connection */
		return;
	}

	/* find the correct channel, and the coordinates to index into it for both the source and
	   destination channels. also return the channel type (ie chanx/chany) into which we are 
	   indexing */
	from_chan_type = index_into_correct_chan(x_coord, y_coord, from_side, chan_details_x, chan_details_y, 
				&from_x, &from_y, &from_chan_details);
	to_chan_type = index_into_correct_chan(x_coord, y_coord, to_side, chan_details_x, chan_details_y, 
				&to_x, &to_y, &to_chan_details);

	/* make sure from_x/y and to_x/y aren't out of bounds */
	if (coords_out_of_bounds(nx, ny, to_x, to_y, to_chan_type) ||
	    coords_out_of_bounds(nx, ny, from_x, from_y, from_chan_type)){
		return;
	}

	/* get the direction in which the source/destination tracks are headed */
	src_direction = get_src_direction(from_side, directionality);
	dest_direction = get_dest_direction(to_side, directionality);	

	/* name of the source track */
	const char *track_name = from_chan_details[from_x][from_y][from_track].type_name_ptr;
	/* iterate over all the wire connections specified by the passed-in switchblock */
	for (int iconn = 0; iconn < (int)sb->wireconns.size(); iconn++){
		/* pointer to a connection specification between two wire types/groups */
		t_wireconn_inf *wireconn_ptr = &(sb->wireconns.at(iconn));

		/* name of track type we're connecting from/to */
		vector<string> *from_track_type = &(wireconn_ptr->from_type);
		vector<string> *to_track_type = &(wireconn_ptr->to_type);
		/* the 'seg' coordinates of the from/to channels */
		int from_seg = get_seg_coordinate( from_chan_type, from_x, from_y );
		int to_seg = get_seg_coordinate( to_chan_type, to_x, to_y );
		/* the 'from' wirepoint and 'to' wirepoints at which the connection is made */
		vector<int> from_wirepoint, to_wirepoint;
		/* the current track corresponds to a single wirepoint -- vector will have only one entry */
		from_wirepoint.push_back( get_wirepoint_of_track(nx, ny, from_chan_type,
				from_chan_details[from_x][from_y][from_track], from_seg, from_side) );
		/* there may be multiple destination wirepoints */
		to_wirepoint = wireconn_ptr->to_point;
		/* vectors that will contain indices of the source/dest tracks belonging to the source/dest wirepoints */
		vector<int> potential_src_tracks;
		vector<int> potential_dest_tracks;
		/* the index of the source/destination track within their own wirepoint group */
		int src_track_in_sp, dest_track_in_sp;
		/* the effective destination channel width is the size of the destination track group */
		int dest_W;

		/* check that the current track has one of the types specified by the wire connection */
		bool skip = true;
		for (int itype = 0; itype < (int)from_track_type->size(); itype++){
			if ( strcmp(from_track_type->at(itype).c_str(), track_name) == 0 ){
				skip = false;
				break;
			}
		}
		if (skip){
			continue;
		}

		/* check that the current track has one of the source wire points specified by the wire connection */
		skip = true;
		for (int ipoint = 0; ipoint < (int)wireconn_ptr->from_point.size(); ipoint++){
			if (from_wirepoint.at(0) == wireconn_ptr->from_point.at(ipoint)){
				skip = false;
				break;
			}
		}
		if (skip){
			continue;
		}

		/* now we need to find all tracks in the destination channel that correspond to the 
		   type and group specified by the current wireconn_ptr. Must also go in appropriate 
		   direction (from e_direction) */

		//Do outgoing unidir tracks get here?

		/* get the indices of tracks in the destination group, as well as the effective destination 
		   channel width (which is the number of tracks in destination group) */
		get_wirepoint_tracks(directionality, nx, ny, x_coord, y_coord, to_side, to_track_type, &to_wirepoint, track_type_sizes, 
				wirepoint_starts, true, &potential_dest_tracks);
		//get_wirepoint_tracks(nx, ny, to_chan_details[to_x][to_y], to_chan_type, to_track_type, &to_wirepoint, to_seg, 
		//				dest_direction, nodes_per_chan, track_type_sizes, to_side, &potential_dest_tracks);
		if (0 == potential_dest_tracks.size()){
			continue;
		}
		dest_W = potential_dest_tracks.size();

		/* get the index of the source track relative to all the tracks specified by the wireconn */
		//get_wirepoint_tracks(nx, ny, from_chan_details[from_x][from_y], from_chan_type, from_track_type, &(wireconn_ptr->from_point), 
		//				from_seg, src_direction, nodes_per_chan, track_type_sizes, from_side,
		//				&potential_src_tracks);
		get_wirepoint_tracks(directionality, nx, ny, x_coord, y_coord, from_side, from_track_type, &(wireconn_ptr->from_point),
				track_type_sizes, wirepoint_starts, false, &potential_src_tracks);
		if (0 == potential_src_tracks.size()){
			continue;
		}
		vector<int>::iterator it = find(potential_src_tracks.begin(), potential_src_tracks.end(), from_track);
		src_track_in_sp = it - potential_src_tracks.begin();
		
		/* get a reference to the string vector containing desired side1->side2 permutations */
		vector<string> &permutations_ref = sb->permutation_map.at(side_conn);

		/* iterate over the permutation functions specified in the string vector */
		int to_track;
		s_formula_data formula_data;
		for (int iperm = 0; iperm < (int)permutations_ref.size(); iperm++){
			/* Convert the symbolic permutation formula to a number */
			formula_data.dest_W = dest_W;
			if (0 == dest_W){
				continue;
			}
			formula_data.track = src_track_in_sp;
			dest_track_in_sp = get_sb_formula_result(permutations_ref.at(iperm).c_str(), formula_data);
			dest_track_in_sp = adjust_formula_result(dest_track_in_sp, dest_W);
			//dest_track_in_sp = (dest_track_in_sp + dest_W) % (dest_W);
			/* the resulting track number is the *index* of the destination track in it's own
			   group, so we need to convert that back to the absolute index of the track in the channel */
			if(dest_track_in_sp < 0){
				vpr_printf(TIO_MESSAGE_ERROR, "compute_track_connections: got a negative track, %d  based on source track %d. From side %d to side %d. W=%d. Permutation function: %s\n", dest_track_in_sp, src_track_in_sp, from_side, to_side, nodes_per_chan, permutations_ref.at(iperm).c_str());
				exit(1);
			}
			to_track = potential_dest_tracks.at(dest_track_in_sp);
			
			/* create the struct containing the to_track and switch name, which will be added to the 
			   sb connections map */	
			t_to_track_inf to_track_inf;
			to_track_inf.to_track = to_track;
			to_track_inf.switch_name = sb->switch_name;
			/* and now, finally, add this switchblock connection to the switchblock connections map */
			(*sb_conns)[sb_conn].push_back(to_track_inf);
			/* If bidir architecture, implement the reverse connection as well */
			if (BI_DIRECTIONAL == directionality){
				to_track_inf.to_track = from_track;
				Switchblock_Lookup sb_conn_reverse(x_coord, y_coord, to_side, from_side, to_track);
				(*sb_conns)[sb_conn_reverse].push_back(to_track_inf);
			}
		}
	}

	track_name = NULL;
	
	from_chan_details = NULL;
	to_chan_details = NULL;
	return;
}


/* Here we find the correct channel (x or y), and the coordinates to index into it based on the 
   specified tile coordinates and the switchblock side. Also returns the type of channel
   that we are indexing into (ie, CHANX or CHANY */
static t_rr_type index_into_correct_chan(INP int tile_x, INP int tile_y, INP enum e_side side, 
			INP t_chan_details *chan_details_x, INP t_chan_details *chan_details_y,
			OUTP int *set_x, OUTP int *set_y, 
			OUTP t_chan_details **set_chan_details){
	
	t_rr_type chan_type;

	/* here we use the VPR convention that a tile 'owns' the channels directly to the right
	   and above it */
	switch (side){
		case TOP:
			/* this is y-channel belonging to tile above */
			*set_chan_details = chan_details_y;
			*set_x = tile_x;
			*set_y = tile_y+1;
			chan_type = CHANY;
			break;
		case RIGHT:
			/* this is x-channel belonging to tile to the right */
			*set_chan_details = chan_details_x;
			*set_x = tile_x+1;
			*set_y = tile_y;
			chan_type = CHANX;
			break;
		case BOTTOM:
			/* this is y-channel on the right of the tile */
			*set_chan_details = chan_details_y;
			*set_x = tile_x;
			*set_y = tile_y;
			chan_type = CHANY;
			break;
		case LEFT:
			/* this is x-channel on top of the tile */
			*set_chan_details = chan_details_x;
			*set_x = tile_x;
			*set_y = tile_y;
			chan_type = CHANX;
			break;
		default:
			vpr_printf(TIO_MESSAGE_ERROR, "index_into_correct_chan: unknown side specified: %d\n", side);
			exit(1);
			break;
	}

	return chan_type;
}

/* checks whether the specified coordinates are out of bounds */
static bool coords_out_of_bounds(INP int nx, INP int ny, INP int x_coord, INP int y_coord, 
			e_rr_type chan_type){
	bool result = true;

	if (CHANX == chan_type){
		if (x_coord <=0 || x_coord >= nx+1 ||	/* there is no x-channel at x=0 */
		    y_coord < 0 || y_coord >= ny+1){
			result = true;
		} else {
			result = false;
		}
	} else if (CHANY == chan_type){
		if (x_coord < 0 || x_coord >= nx+1 ||
		    y_coord <= 0 || y_coord >= ny+1){	/* there is no y-channel at y=0 */
			result = true;
		} else {
			result = false;
		}

	} else {
		vpr_printf(TIO_MESSAGE_ERROR, "coords_out_of_bounds(): illegal channel type %d\n", chan_type);
		exit(1);
	}
	return result;
}

/* We can look at a channel as either having an x and y coordinate, or as having a 
   'seg' and 'chan' coordinate. Here we return the seg coordinate based on the 
   channel type and channel x/y coordinates */
static int get_seg_coordinate(INP t_rr_type chan_type, INP int x_coord, INP int y_coord){
	int seg = -1;
	
	switch (chan_type){
		case CHANX:
			seg = x_coord;
			break;
		case CHANY:
			seg = y_coord;
			break;
		default:
			vpr_printf(TIO_MESSAGE_ERROR, "get_seg_coordinate: 'chan_type' %d is not a channel\n", chan_type);
			exit(1);
			break;
	}

	return seg;
}

/* returns the direction in which a source track at the specified
   switchblock side should be headed. i.e. if we connect from a track on the top
   side of a switchblock in a unidir architecture, this track should be
   going in the -y direction (aka in the direction of 'decreasing' coordinate) . */
static e_direction get_src_direction(e_side from_side, e_directionality directionality){
	e_direction src_dir;

	if (BI_DIRECTIONAL == directionality){
		src_dir = BI_DIRECTION;
	} else if (UNI_DIRECTIONAL == directionality){
		if (TOP == from_side || RIGHT == from_side){
			/* in direction of decreasing coordinate */
			src_dir = DEC_DIRECTION;
		} else if (LEFT == from_side || BOTTOM == from_side){
			/* in direction of increasing coordinate */
			src_dir = INC_DIRECTION;
		} else {
			vpr_printf(TIO_MESSAGE_ERROR, "get_src_direction(): unkown switchblock side %d\n", from_side);
			exit(1);
		}
	} else {
		vpr_printf(TIO_MESSAGE_ERROR, "get_src_direction(): unknown directionality %d\n", directionality);
		exit(1);
	}

	return src_dir;
}

/* returns the direction in which a destination track at the specified
   switchblock side should be headed. i.e. if we connect to a track on the top
   side of a switchblock in a unidir architecture, this track should be
   going in the +y direction (aka in the direction of 'increasing' coordinate) . */
static e_direction get_dest_direction(e_side to_side, e_directionality directionality){
	e_direction dest_dir;

	if (BI_DIRECTIONAL == directionality){
		dest_dir = BI_DIRECTION;
	} else if (UNI_DIRECTIONAL == directionality){
		if (TOP == to_side || RIGHT == to_side){
			/* in direction of increasing coordinate */
			dest_dir = INC_DIRECTION;
		} else if (LEFT == to_side || BOTTOM == to_side){
			/* in direction of decreasing coordinate */
			dest_dir = DEC_DIRECTION;
		} else {
			vpr_printf(TIO_MESSAGE_ERROR, "get_dest_direction(): unkown switchblock side %d\n", to_side);
			exit(1);
		}
	} else {
		vpr_printf(TIO_MESSAGE_ERROR, "get_dest_direction(): unknown directionality %d\n", directionality);
		exit(1);
	}

	return dest_dir;
}

/* returns the group number of the specified track */
static int get_track_group(INP int nx, INP int ny, INP e_rr_type chan_type,
			 INP t_seg_details &track_details, int seg_coord){
	
	/* We get track group by comparing the track's seg_coord to the seg_start of the track.
	   The offset between seg_start and seg_coord is the group number */

	/* ISSUE: seg start is the actual start of the segment. i.e. at seg=1, all tracks have a 
	start point at seg=1. 
	One solution is to look at both seg_start and seg_end. Is that enough info?
	Cases:
	seg starts at bottom but does not extend all the way to the top -- look at seg_end
	seg starts > bottom and does not extend all the way to top -- look at seg_start or seg_end
	seg starts > bottom but terminates all the way at the top -- look at seg_start
	seg starts at bottom and extends all the way to the top -- must be group 0
	*/


	int group;
	int seg_start = track_details.seg_start;
	int seg_end = track_details.seg_end;
	int track_length = track_details.length;
	int max_seg, min_seg;

	/* determine the minimum and maximum values that the 'seg' coordinate 
	   of a track can take */
	min_seg = 1;	
	if (CHANX == chan_type){
		max_seg = nx;
	} else {
		assert(CHANY == chan_type);
		max_seg = ny;
	}

	if (seg_start != min_seg){
		group = seg_coord - seg_start;
	} else if (seg_end != max_seg){
		group = (track_length-1) - (seg_end - seg_coord);
	} else {
		/* track must span the entire FPGA */
		group = 0;
	}

	/* the start of this track cannot be *after* the current seg coordinate */
	if (group < 0){
		vpr_printf(TIO_MESSAGE_ERROR, "get_track_group: invalid seg_start, seg_coord combination\n");
		exit(1); 
	}

	return group;
}

/* Finds the index of the first track in the channel specified by 'track_details'
   to have the type name specified by 'type' */
static int find_start_of_track_type(INP t_seg_details *track_details, INP const char *type, 
			INP int nodes_per_chan, INP t_track_type_sizes *track_type_sizes){

	string map_accessor;
	int track = 0;
	while( track < nodes_per_chan ){
		/* check type of track */
		if ( strcmp(type, track_details[track].type_name_ptr) == 0 ){
			/* track type found */
			break;
		} else {
			/* move on to next track type */
			map_accessor = track_details[track].type_name_ptr;
			track += track_type_sizes->at(map_accessor).num_tracks;
		}
	}

	if (track >= nodes_per_chan){
		vpr_printf(TIO_MESSAGE_ERROR, "find_start_of_track_type: track type %s could not be found in the channel\n", type);
		exit(1);
	}

	return track;
} 

/* returns the track index in the channel specified by 'track_details' that corresponds to the 
   first occurence of the specified track group (belonging to type referred to by track_type_start, 
   track_group, and track_direction). returns -1 if the requested track group can not be found */
static int find_start_of_track_group(INP int nx, INP int ny, INP t_rr_type chan_type, 
			INP t_seg_details *track_details, INP const char *type, 
			INP int track_group, INP int track_seg_coord, INP e_direction track_direction, 
			INP int track_type_start, INP int nodes_per_chan, INP t_track_type_sizes *track_type_sizes){
	string map_accessor = type;
	int num_tracks = track_type_sizes->at(map_accessor).num_tracks;
	int track_type_end = track_type_start + num_tracks - 1;	

	int track;
	for (track = track_type_start; track <= track_type_end; track++){
		e_direction direction = track_details[track].direction;
		int group = get_track_group(nx, ny, chan_type, track_details[track], track_seg_coord);
		if (group == track_group && direction == track_direction){
			/* found the requested track group */
			break;
		}

		/* if we hit the last track but still haven't found the group... */
		if (track == track_type_end){
			//vpr_printf(TIO_MESSAGE_ERROR, "track group %d could not be found in the channel\n", track_group);
			//exit(1);
			track = -1;
		}
	}

	return track;
}

/* fills the 'tracks_in_group' vector with the indices of the tracks in the 'track_details' channel
   which have the specified 'track_type', 'track_group', and are going in 'track_direction'. */
static void get_group_tracks(INP int nx, INP int ny, INP t_seg_details *track_details, INP t_rr_type chan_type,
			INP const char *track_type, INP int track_group, INP int track_seg_coord, 
			INP e_direction track_direction, INP int nodes_per_chan, INP int type_start, 
			INP t_track_type_sizes *track_type_sizes, INOUTP vector<int> *tracks){

	int group_size;			/* the number of tracks in the channel belonging to specified type->group */
	int group_start;		/* track index at which the given 'group' starts */
	int track_type_end;		/* track index where the track type ends */
	int track_length;		/* length of track */
	int dir_mult;			/* multiplier to account for unidir/bidir differences */
	int step_size;			/* spacing between two consecutive tracks of the same group/type */
	int num_tracks;			/* the total number of tracks which will be put int 'tracks' */
	int track_start;		/* the index of the first track */
	
	/* multiplier to account for unidir vs bidir differences */
	if (BI_DIRECTION == track_direction){
		dir_mult = 1;
	} else if (INC_DIRECTION == track_direction || DEC_DIRECTION == track_direction){
		dir_mult = 2;
	} else {
		vpr_printf(TIO_MESSAGE_ERROR, "get_group_size(): unknown track direction %d\n", track_direction);
		exit(1);
	}
	
	string map_accessor = track_type;
	/* considering tracks which are currently at a certain wirepoint */
	/* get index of track in channel at which the specified type->group starts */
	group_start = find_start_of_track_group(nx, ny, chan_type, track_details, track_type, track_group, 
					track_seg_coord, track_direction, type_start, nodes_per_chan, 
					track_type_sizes);
	
	if (-1 == group_start){
		/* could not find specified group, do nothing */
	} else {
		track_type_end = type_start + track_type_sizes->at(map_accessor).num_tracks - 1;
		track_length = track_details[group_start].length;

		/* get the group size. Consecutive tracks of the same group
		   are separated by 'track_length' tracks for the bidirectional case,
		   and by 2*track_length tracks for the unidirectional case, because for 
		   unidir tracks are grouped in pairs (the dir_mult factor accounts for this). */
		step_size = track_length * dir_mult;
		group_size = track_type_end - group_start;
		group_size = floor(group_size / step_size) + 1;
		num_tracks = group_size;
		track_start = group_start;

		/* and now we set which track indices in the channel correspond to the 
		   desired type, group, and directionality */
		tracks->reserve(num_tracks);
		for (int i = 0; i < num_tracks; i++){
			tracks->push_back( track_start + i*step_size );
		}
	}

	return;
}

/* Returns the wirepoint of the track specified by track_details at a segment coordinate
   of seg_coord, and connection to the sb_side of the switchblock */
static int get_wirepoint_of_track(INP int nx, INP int ny, INP e_rr_type chan_type,
		 INP t_seg_details &track_details, INP int seg_coord, INP e_side sb_side){

	/* this function calculates the wirepoint of a given track by first calculating
	   the group of the specified track. For instance, for a track with L=4:

		wirepoint:	0-------1-------2-------3-------0
		      group:	    0       1       2       3

	   So knowing the track's group and which switchblock side it connects to is 
	   enough to calculate the wirepoint

	*/

	int wirepoint;
	int track_length = track_details.length;
	int group = get_track_group(nx, ny, chan_type, track_details, seg_coord);

	if (LEFT == sb_side || BOTTOM == sb_side){
		wirepoint = (group + 1) % track_length;
	} else {
		assert(RIGHT == sb_side || TOP == sb_side);
		wirepoint = group;
	}

	return wirepoint;
}

/* in this function we'd like to find all the tracks belonging to the specified wirepoint.
   since the wirepoint is related to the track group as:

	wirepoint:	0-------1-------2-------3-------0
	      group:	    0       1       2       3

   we can find the tracks belonging to the wirepoint by finding the tracks belonging to
   the appropriate group.
   The result is stored in the 'tracks' vector */
static void get_wirepoint_tracks(INP int nx, INP int ny, INP t_seg_details *track_details, INP t_rr_type chan_type,
			INP const char *track_type, INP vector<int> *track_wirepoint, INP int track_seg_coord, 
			INP e_direction track_direction, INP int nodes_per_chan, INP t_track_type_sizes *track_type_sizes, 
			INP e_side sb_side, INOUTP vector<int> *tracks){

	int track_group;
	int type_start = find_start_of_track_type(track_details, track_type, nodes_per_chan, track_type_sizes);
	int track_length = track_details[type_start].length;

	/* more than one wirepoint may exist in the passed-in vector. account for each one */
	for (int ipoint = 0; ipoint < (int)track_wirepoint->size(); ipoint++){

		/* get the track group based on the desired wirepoint */
		if (LEFT == sb_side || BOTTOM == sb_side){
			track_group = (track_wirepoint->at(ipoint) - 1 + track_length) % track_length ;
		} else {
			assert(RIGHT == sb_side || TOP == sb_side);
			track_group = track_wirepoint->at(ipoint);
		}

		/* fill 'tracks' with the indices of the track corresponding to the desired group/wirepoint */
		get_group_tracks(nx, ny, track_details, chan_type, track_type, track_group, track_seg_coord, track_direction, 
			nodes_per_chan, type_start, track_type_sizes, tracks);
	}
	
	/* since 'tracks' was filled one wirepoint at a time, it is now necessary to sort 'tracks' in ascending order
	   so that the entries come in the order of track indices */
	sort(tracks->begin(), tracks->end());

	return;
}

/* adjusts a negative destination track calculated from a permutation formula */
static int adjust_formula_result(INP int dest_track, INP int dest_W){
	int result = dest_track;
	if (dest_track < 0){
		int mult = (-1*dest_track) / dest_W + 1;
		result = dest_track + mult*dest_W;
	}
	result = (result + dest_W) % dest_W;
	return result;
}

