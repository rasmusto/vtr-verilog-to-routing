#ifndef PARSE_SWITCHBLOCKS_H
#define PARSE_SWITCHBLOCKS_H

/**** Includes ****/
#include "ezxml.h"

/**** Enums ****/


/**** Structs ****/
/* contains data passed in to the switchblock parser */
struct s_formula_data{
	int dest_W; 	/* W in destination channel */
	int track;	/* incoming track index */
	int Fs;
	int Fc_in;
	int Fc_out;
};



/**** Classes ****/
class Switchblock_Lookup{
public:
	int x_coord;
	int y_coord;
	int from_side;
	int to_side;
	int track_num;	/* from track */

	/* Empty constructor initializes everything to 0 */
	Switchblock_Lookup(){
		x_coord = y_coord = from_side = to_side = track_num = 0;
	}

	/* Constructor for initializing member variables */
	Switchblock_Lookup(int set_x, int set_y, int set_from, int set_to, int set_track){
		x_coord = set_x;
		y_coord = set_y;
		from_side = set_from;
		to_side = set_to;
		track_num = set_track;
	}

	/* Function for setting the segment coordinates */
	void set_coords(int set_x, int set_y, int set_from, int set_to, int set_track){
		x_coord = set_x;
		y_coord = set_y;
		from_side = set_from;
		to_side = set_to;
		track_num = set_track;
	}

	/* Overload < operator which is used by std::map */
	bool operator < (const Switchblock_Lookup &obj) const {
		bool result;

		/* This is ugly, but it implements a hierarchy of inequality. For instance, if x1 < x2 then
		   obj1 < obj2. But if x1==x2 then we have to check y1 and y2, and so on */
		if (x_coord < obj.x_coord){
			result = true;
		} else {
			if (x_coord == obj.x_coord){
				if (y_coord < obj.y_coord){
					result = true;
				} else {
					if (y_coord == obj.y_coord){
						if (from_side < obj.from_side){
							result = true;
						} else {
							if (from_side == obj.from_side){
								if (to_side < obj.to_side){
									result = true;
								} else {
									if (to_side == obj.to_side){
										if (track_num < obj.track_num){
											result = true;
										} else {
											result = false;
										}
									} else {
										result = false;
									}
								}
							} else {
								result = false;
							}
						}
					} else {
						result = false;
					}
				}
			} else {
				result = false;
			}
		}
		
		return result;
	}
}; /* class Switchblock_Lookup */
std::ostream& operator<<(std::ostream &os, const Switchblock_Lookup &obj);


/**** Typedefs ****/
typedef struct s_to_track_inf{
	int to_track;
	std::string switch_name;
} t_to_track_inf;

/* Switchblock connections are made as [0..nx][0..ny][from_side][to_side][from_track (0..W-1)].
   We use the Switchblock_Lookup class to specify these five dimensions.
   Furthermore, a source_track at a given 5-d coordinate *may* connect to multiple destination tracks. We use a 
   vector to represent this possibility.
   A matrix specifying connections for all switchblocks in an FPGA would be very large, and probably sparse,
   so we use a map to take advantage of the sparsity. */
typedef std::map< Switchblock_Lookup, std::vector< t_to_track_inf > > t_sb_connection_map;



/**** Function Declarations ****/
/* Loads permutation funcs specified under Node into t_switchblock_inf */
void read_sb_switchfuncs( INP ezxml_t Node, INOUTP t_switchblock_inf *sb );

/* Reads-in the wire connections specified for the switchblock in the xml arch file */
void read_sb_wireconns( INP ezxml_t Node, INOUTP t_switchblock_inf *sb );

/* returns integer result according to the specified formula and data */
int get_sb_formula_result( INP const char* formula, INP const s_formula_data &mydata );



#endif /* PARSE_SWITCHBLOCKS_H */
