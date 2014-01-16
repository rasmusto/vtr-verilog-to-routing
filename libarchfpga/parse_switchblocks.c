/* 
===== Description =====
The contents of this file assist in parsing switchblock descriptions specified
in the XML architecture file.

A large chunk of this file is dedicated to helping parse the initial switchblock
specificaiton in the XML arch file, providing error checking, etc.

Another large chunk of this code is dedicated to parsing the actual formulas 
specified by the switchblock permutation functions into their numeric counterparts.
*/


#include <string.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <utility>
#include "util.h"
#include "read_xml_util.h"
#include "arch_types.h"
#include "physical_types.h"
#include "parse_switchblocks.h"

using namespace std;

/**** Enums ****/
/* Used to identify the type of symbolic formula object */
typedef enum e_formula_obj{
	E_FML_UNDEFINED = 0,
	E_FML_NUMBER,
	E_FML_BRACKET,
	E_FML_OPERATOR,
	E_FML_NUM_FORMULA_OBJS
} t_formula_obj;

/* Used to identify an operator in a formula */ 
typedef enum e_operator{
	E_OP_UNDEFINED = 0,
	E_OP_ADD,
	E_OP_SUB,
	E_OP_MULT,
	E_OP_DIV,
	E_OP_NUM_OPS
} t_operator;

/**** Class Definitions ****/
/* This class is used to represent an object in a formula, such as 
   a number, a bracket, an operator, or a variable */
class Formula_Object{
public:
	/* indicates the type of formula object this is */
	t_formula_obj type;

	/* object data, accessed based on what kind of object this is */
	union u_Data {
		int num;		/*for number objects*/
		t_operator op;		/*for operator objects*/
		bool left_bracket;	/*for bracket objects -- specifies if this is a left bracket*/

		u_Data(){ memset(this, 0, sizeof(u_Data)); }
	} data;

	Formula_Object(){
		this->type = E_FML_UNDEFINED;
	}
	
};



/**** Function Declarations ****/

/*---- Functions for Parsing Switchblocks from Architecture ----*/

/* parses the wirepoints specified in ch into the vector wire_points_vec */
static void parse_comma_separated_wire_points(INP const char *ch, INOUTP vector<int> *wire_points_vec);

/* checks for correctness of a unidir switchblock. hard exit if error found (to be changed to throw later) */
static void check_unidir_switchblock( INP t_permutation_map *permutation_map );

/* checks for correctness of a bidir switchblock. hard exit if error found (to be changed to throw later) */
static void check_bidir_switchblock( INP t_permutation_map *permutation_map );



/*---- Functions for Parsing the Symbolic Switchblock Formulas ----*/
/* returns integer result according to specified formula and data */
static int parse_formula( INP const char *formula, INP const s_formula_data &mydata );

/* returns integer result according to specified piece-wise formula and data */
static int parse_piecewise_formula( INP const char *formula, INP const s_formula_data &mydata );

/* converts specified formula to a vector in reverse-polish notation */
static void formula_to_rpn( INP const char* formula, INP const s_formula_data &mydata, 
				INOUTP vector<Formula_Object> &rpn_output );

static void get_formula_object( INP const char *ch, INOUTP int &ichar, INP const s_formula_data &mydata,
				 INOUTP Formula_Object *fobj );

/* returns integer specifying precedence of passed-in operator. higher integer 
   means higher precedence */
static int get_fobj_precedence( INP const Formula_Object &fobj );

/* Returns associativity of the specified operator */
static bool op_associativity_is_left( INP const t_operator &op );

/* used by the shunting-yard formula parser to deal with operators such as add and subtract */
static void handle_operator( INP const Formula_Object &fobj, INOUTP vector<Formula_Object> &rpn_output,
				INOUTP stack<Formula_Object> &op_stack);

/* used by the shunting-yard formula parser to deal with brackets, ie '(' and ')' */
static void handle_bracket( INP const Formula_Object &fobj, INOUTP vector<Formula_Object> &rpn_output,
				INOUTP stack<Formula_Object> &op_stack);

/* parses revere-polish notation vector to return formula result */
static int parse_rpn_vector( INOUTP vector<Formula_Object> &rpn_vec );

/* applies operation specified by 'op' to the given arguments. arg1 comes before arg2 */
static int apply_rpn_op( INP const Formula_Object &arg1, INP const Formula_Object &arg2, 
					INP const Formula_Object &op );

/* checks if specified character represents an ASCII number */
static bool is_char_number( INP const char ch );

/* checks if the specified formula is piece-wise defined */
static bool is_piecewise_formula( INP const char *formula );

/* increments str_ind until it reaches specified char is formula. returns true if character was found, false otherwise */
static bool goto_next_char( INOUTP int *str_ind, INP const string &pw_formula, char ch);


/**** Function Definitions ****/

/*---- Functions for Parsing Switchblocks from Architecture ----*/

/* Reads-in the wire connections specified for the switchblock in the xml arch file */
void read_sb_wireconns( INP ezxml_t Node, INOUTP t_switchblock_inf *sb ){
	
	/* Make sure that Node is a switchblock */
	CheckElement(Node, "switchblock");
	
	int num_wireconns;
	ezxml_t SubElem;
	const char *char_prop;

	/* count the number of specified wire connections for this SB */
	num_wireconns = CountChildren(Node, "wireconn", 1);
	sb->wireconns.reserve(num_wireconns);

	for (int i = 0; i < num_wireconns; i++){
		t_wireconn_inf wc;		

		SubElem = ezxml_child(Node, "wireconn");

		/* get from type */
		char_prop = FindProperty(SubElem, "FT", TRUE);
		wc.from_type = char_prop;
		ezxml_set_attr(SubElem, "FT", NULL);

		/* get to type */
		char_prop = FindProperty(SubElem, "TT", TRUE);
		wc.to_type = char_prop;
		ezxml_set_attr(SubElem, "TT", NULL);

		/* get the source wire point */
		char_prop = FindProperty(SubElem, "FP", TRUE);
		parse_comma_separated_wire_points(char_prop, &(wc.from_point));
		ezxml_set_attr(SubElem, "FP", NULL);

		/* get the destination wire point */
		char_prop = FindProperty(SubElem, "TP", TRUE);
		parse_comma_separated_wire_points(char_prop, &(wc.to_point));
		ezxml_set_attr(SubElem, "TP", NULL);

		sb->wireconns.push_back(wc);
		FreeNode(SubElem);
	}

	return;
}

/* parses the wirepoints specified in the comma-separated 'ch' array into the vector wire_points_vec */
static void parse_comma_separated_wire_points(INP const char *ch, INOUTP vector<int> *wire_points_vec){
	int ind = 0;
	wire_points_vec->clear();

	/* walk through ch and check that all characters are legal */
	while ('\0' != ch[ind]){
		if (',' != ch[ind] && ' ' != ch[ind] && !is_char_number(ch[ind])){
			vpr_printf(TIO_MESSAGE_ERROR, "parse_comma_separated_wire_points: found wireconn wire point entry with illegal character: %c\n", ch[ind]);
			exit(1);
		}
		ind++;
	}
	if (0 == ind){
		vpr_printf(TIO_MESSAGE_ERROR, "parse_comma_separated_wire_points: found empty wireconn wire point entry\n");
		exit(1);
	}

	/* error checking done, move on to parsing */ 
	string points(ch);
	int str_size = points.size();
	int ch_start = 0;
	ind = 0;
	while (ch_start <= str_size - 1){
		string substr;
		
		/* get the next wirepoint */
		if (ind != str_size - 1){
			goto_next_char(&ind, points, ',');
		}
		if (ind == str_size - 1){
			substr = points.substr(ch_start, ind - ch_start + 1);
		} else {
			substr = points.substr(ch_start, ind - ch_start);
		}
		wire_points_vec->push_back( atoi(substr.c_str()) );

		ch_start = ind + 1;
	}

	return;
}

/* Loads permutation funcs specified under Node into t_switchblock_inf. Node should be 
   <switchfuncs> */
void read_sb_switchfuncs( INP ezxml_t Node, INOUTP t_switchblock_inf *sb ){
	
	/* Make sure the passed-in is correct */
	CheckElement(Node, "switchfuncs");
	
	ezxml_t SubElem;

	/* Used to designate if a predifined function such as 'wilton' has been found.
	   If a predifined function is specified, only one function entry is allowed */
	bool predefined_sb_found = false;

	/* get directionality */
	enum e_directionality directionality = sb->directionality;
	
	/* get the number of specified permutation functions */
	int num_funcs = CountChildren(Node, "func", 1);

	const char * func_type;
	const char * func_formula;
	vector<string> * func_ptr;

	/* used to index into permutation map of switchblock */
	Connect_SB_Sides conn;

	/* now we iterate through all the specified permutation functions, and 
	   load them into the switchblock structure as appropriate */
	for (int ifunc = 0; ifunc < num_funcs; ifunc++){
		/* get the next switchblock function */
		SubElem = ezxml_child(Node, "func");
		/* get function type */
		func_type = FindProperty(SubElem, "type", TRUE);
		if (!func_type){
			vpr_printf(TIO_MESSAGE_ERROR, "empty function specification in switchblock\n");
			exit(1);
		}
		ezxml_set_attr(SubElem, "type", NULL);
		/* get function formula */
		func_formula = FindProperty(SubElem, "formula", TRUE);
		if (!func_formula){
			vpr_printf(TIO_MESSAGE_ERROR, "empty formula specification in switchblock\n");
			exit(1);
		}
		ezxml_set_attr(SubElem, "formula", NULL);


		/* a predefined function should be the only entry */
		if (predefined_sb_found && ifunc > 0){
			vpr_printf(TIO_MESSAGE_ERROR, "Predefined switchblock function should be the only entry in the switchfuncs section. Other entry found: %s = %s\n", func_type, func_formula);
			exit(1);
		}

		/* go through all the possible cases of func_type */
		if (0 == strcmp(func_type, "lt")){
			conn.set_sides(LEFT, TOP);
		} else if (0 == strcmp(func_type, "lr")) {
			conn.set_sides(LEFT, RIGHT);
		} else if (0 == strcmp(func_type, "lb")) {
			conn.set_sides(LEFT, BOTTOM);
		} else if (0 == strcmp(func_type, "tl")) {
			conn.set_sides(TOP, LEFT);
		} else if (0 == strcmp(func_type, "tb")) {
			conn.set_sides(TOP, BOTTOM);
		} else if (0 == strcmp(func_type, "tr")) {
			conn.set_sides(TOP, RIGHT);
		} else if (0 == strcmp(func_type, "rt")) {
			conn.set_sides(RIGHT, TOP);
		} else if (0 == strcmp(func_type, "rl")) {
			conn.set_sides(RIGHT, LEFT);
		} else if (0 == strcmp(func_type, "rb")) {
			conn.set_sides(RIGHT, BOTTOM);
		} else if (0 == strcmp(func_type, "bl")) {
			conn.set_sides(BOTTOM, LEFT);
		} else if (0 == strcmp(func_type, "bt")) {
			conn.set_sides(BOTTOM, TOP);
		} else if (0 == strcmp(func_type, "br")) {
			conn.set_sides(BOTTOM, RIGHT);
		} else if (0 == strcmp(func_type, "predefined")){
			/* a predifined permutation function */
			predefined_sb_found = true;
		} else {
			/* unknown permutation function */
			vpr_printf(TIO_MESSAGE_ERROR, "Unknown permutation function specified: %s\n", func_type);
			exit(1);
		}
		func_ptr = &(sb->permutation_map[conn]);

		/* Here we load the specified switch function(s) */
		if (predefined_sb_found){
			//TODO: load_predefined_switchfuncs( const char *predef_switch, ...? );
		} else {
			string tmp(func_formula);
			func_ptr->push_back( tmp );
		}
 
		func_ptr = NULL;
		FreeNode(SubElem);
	}
	
	/* Check for errors in the switchblock descriptions */
	if (UNI_DIRECTIONAL == directionality){
		check_unidir_switchblock( &(sb->permutation_map) );
	} else {
		assert(BI_DIRECTIONAL == directionality);
		check_bidir_switchblock( &(sb->permutation_map) );
	}


	return;
} /* read_sb_switchfuncs */

/* checks for correctness of a unidir switchblock. hard exit if error found (to be changed to throw later) */
static void check_unidir_switchblock( INP t_permutation_map *permutation_map ){
	//nothing much to do here, everything is legal...
}

/* checks for correctness of a bidir switchblock. hard exit if error found (to be changed to throw later) */
static void check_bidir_switchblock( INP t_permutation_map *permutation_map ){
	/* check that if side1->side2 is specified, then side2->side1 is not, as it is implicit */

	/* variable used to index into the permutation map */
	Connect_SB_Sides conn;

	/* iterate over all combinations of from_side -> to side */
	for ( e_side from_side = (e_side) 0; from_side < 4; from_side = (e_side)(from_side + 1) ){
		for ( e_side to_side = (e_side) 0; to_side < 4; to_side = (e_side)(to_side + 1) ){
			/* can't connect a switchblock side to itself */
			if (from_side == to_side){
				continue;
			}

			/* index into permutation map with this variable */			
			conn.set_sides(from_side, to_side);

			/* check if a connection between these sides exists */
			t_permutation_map::const_iterator it = (*permutation_map).find(conn);
			if (it != (*permutation_map).end()){
				/* the two sides are connected */
				/* check if the opposite connection has been specified */
				conn.set_sides(to_side, from_side);
				it = (*permutation_map).find(conn);
				if (it != (*permutation_map).end()){
					vpr_printf(TIO_MESSAGE_ERROR, "in check_bidir_switchblock: if a connection from side1->side2 was specified, no connection should have been specified from side2->side1 as it is implicit. But such a specification was found.\n");
					exit(1);
				}
			}
		}
	}

	return;
}





/*---- Functions for Parsing the Symbolic Switchblock Formulas ----*/

/* returns integer result according to the specified switchblock formula and data. formula may be piece-wise */
int get_sb_formula_result( INP const char* formula, INP const s_formula_data &mydata ){
	/* the result of the formula will be an integer */
	int result = -1;

	/* check formula */
	if (NULL == formula){
		vpr_printf(TIO_MESSAGE_ERROR, "in get_sb_formula_result: SB formula pointer NULL\n");
		exit(1);
	} else if ('\0' == formula[0]){
		vpr_printf(TIO_MESSAGE_ERROR, "in get_sb_formula_result: SB formula empty\n");
		exit(1);
	}

	/* parse based on whether formula is piece-wise or not */
	if ( is_piecewise_formula(formula) ){
		result = parse_piecewise_formula( formula, mydata );
	} else {
		result = parse_formula( formula, mydata );
	}
	
	return result;
}

/* returns integer result according to specified non-piece-wise formula and data */
static int parse_formula( INP const char *formula, INP const s_formula_data &mydata ){
	int result = -1;

	/* output in reverse-polish notation */
	vector<Formula_Object> rpn_output;	

	/* now we have to run the shunting-yard algorithm to convert formula to reverse polish notation */
	formula_to_rpn( formula, mydata, OUTP rpn_output );
	
	/* then we run an RPN parser to get the final result */
	result = parse_rpn_vector(rpn_output);
	
	return result;
}

/* returns integer result according to specified piece-wise formula and data */
static int parse_piecewise_formula( INP const char *formula, INP const s_formula_data &mydata ){
	int result = -1;
	int str_ind = 0;
	int str_size = 0;
	int t = mydata.track;
	int tmp_ind_start = -1;
	int tmp_ind_count = -1;
	string substr;
	
	/* convert formula to string format */
	string pw_formula(formula);
	str_size = pw_formula.size();

	if (pw_formula.at(str_ind) != '{'){
		vpr_printf(TIO_MESSAGE_ERROR, "parse_piecewise_formula: the first character in piece-wise formula should always be '{'\n");
		exit(1);
	}
	
	/* find the range to which t corresponds */
	/* the first character must be '{' as verified above */
	while (str_ind != str_size - 1){
		/* set to true when range to which track number corresponds has been found */
		bool found_range = false;
		bool char_found = false;
		int range_start = -1;
		int range_end = -1;
		tmp_ind_start = -1;
		tmp_ind_count = -1;

		/* get the start of the range */
		tmp_ind_start = str_ind + 1;
		char_found = goto_next_char(&str_ind, pw_formula, ':');
		if (!char_found){
			vpr_printf(TIO_MESSAGE_ERROR, "parse_piecewise_formula: could not find char %c\n", ':');
			exit(1);
		}
		tmp_ind_count = str_ind - tmp_ind_start;			/* range start is between { and : */
		substr = pw_formula.substr(tmp_ind_start, tmp_ind_count);
		range_start = parse_formula(substr.c_str(), mydata);
	
		/* get the end of the range */
		tmp_ind_start = str_ind + 1;
		char_found = goto_next_char(&str_ind, pw_formula, '}');
		if (!char_found){
			vpr_printf(TIO_MESSAGE_ERROR, "parse_piecewise_formula: could not find char %c\n", '}');
			exit(1);
		}
		tmp_ind_count = str_ind - tmp_ind_start;			/* range end is between : and } */
		substr = pw_formula.substr(tmp_ind_start, tmp_ind_count);
		range_end = parse_formula(substr.c_str(), mydata);

		if (range_start > range_end){
			vpr_printf(TIO_MESSAGE_ERROR, "parse_piecewise_formula: range_start, %d, is bigger than range end, %d\n", range_start, range_end);
			exit(1);
		}

		/* is the incoming track within this range? (inclusive) */
		if ( range_start <= t && range_end >= t ){
			found_range = true;
		} else {
			found_range = false;
		}
			
		/* we're done if found correct range */
		if (found_range){
			break;
		}
		char_found = goto_next_char(&str_ind, pw_formula, '{');
		if (!char_found){
			vpr_printf(TIO_MESSAGE_ERROR, "parse_piecewise_formula: could not find char %c\n", '{');
			exit(1);
		}
	}
	/* the string index should never actually get to the end of the string because we should have found the range to which the 
	   current track number corresponds */
	if (str_ind == str_size-1){
		vpr_printf(TIO_MESSAGE_ERROR, "parse_piecewise_formula: could not find a closing '}'?\n");
		exit(1);
	}

	/* at this point str_ind should point to '}' right before the formula we're interested in starts */
	/* get the value corresponding to this formula */
	tmp_ind_start = str_ind + 1;
	goto_next_char(&str_ind, pw_formula, ';');
	tmp_ind_count = str_ind - tmp_ind_start;			/* formula is between } and ; */
	substr = pw_formula.substr(tmp_ind_start, tmp_ind_count);
	result = parse_formula(substr.c_str(), mydata);

	return result;
}

/* increments str_ind until it reaches specified char is formula. returns true if character was found, false otherwise */
static bool goto_next_char( INOUTP int *str_ind, INP const string &pw_formula, char ch){
	bool result = true;
	int str_size = pw_formula.size();	
	if ((*str_ind) == str_size-1){
		vpr_printf(TIO_MESSAGE_ERROR, "goto_next_char: passed-in str_ind is already at the end of string\n");
		exit(1);
	}

	do{
		(*str_ind)++;
		if ( pw_formula.at(*str_ind) == ch ){
			/* found the next requested character */
			break;
		}

	} while ((*str_ind) != str_size-1);
	if ((*str_ind) == str_size-1 && pw_formula.at(*str_ind) != ch){
		result = false;
	}
	return result;
}

/* Parses the specified formula using a shunting yard algorithm (see wikipedia). The function's result 
   is stored in the rpn_output vector in reverse-polish notation */
static void formula_to_rpn( INP const char* formula, INP const s_formula_data &mydata, 
				INOUTP vector<Formula_Object> &rpn_output ){

	stack<Formula_Object> op_stack;		/* stack for handling operators and brackets in formula */
	Formula_Object fobj;		 	/* for parsing formula objects */

	int ichar = 0;
	const char *ch = NULL;
	/* go through formula and build rpn_output along with op_stack until \0 character is hit */
	while(1) {
		ch = &formula[ichar];

		if ('\0' == (*ch)){
			/* we're done */
			break;
		} else if (' ' == (*ch)){
			/* skip space */
		} else {
			/* parse the character */
			get_formula_object( ch, ichar, mydata, &fobj );
			switch (fobj.type){
				case E_FML_NUMBER:
					/* add to output vector */
					rpn_output.push_back( fobj );
					break;
				case E_FML_OPERATOR:
					/* operators may be pushed to op_stack or rpn_output */
					handle_operator( fobj, rpn_output, op_stack);
					break;
				case E_FML_BRACKET:
					/* brackets are only ever pushed to op_stack, not rpn_output */
					handle_bracket( fobj, rpn_output, op_stack);
					break;
				default:
					vpr_printf(TIO_MESSAGE_ERROR, "in formula_to_rpn: unknown formula object type: %d\n", fobj.type);
					break;
			}
		}
		ichar++;
	}

	/* pop all remaining operators off of stack */
	Formula_Object fobj_dummy;
	while ( !op_stack.empty() ){
		fobj_dummy = op_stack.top();

		if (E_FML_BRACKET == fobj_dummy.type){
			vpr_printf(TIO_MESSAGE_ERROR, "in formula_to_rpn: Mismatched brackets in user-provided formula\n");
			exit(1);
		}		

		rpn_output.push_back( fobj_dummy );
		op_stack.pop();
	}

	return;
}

/* Fills the formula object fobj according to specified character and mydata, 
   which help determine which numeric value, if any, gets assigned to fobj
   ichar is incremented by the corresponding count if the need to step through the 
   character array arises */
static void get_formula_object( INP const char *ch, INOUTP int &ichar, INP const s_formula_data &mydata,
				 INOUTP Formula_Object *fobj ){

	/* the character can either be part of a number, or it can be an object like W, t, (, +, etc
	   here we have to account for both possibilities */

	if ( is_char_number(*ch) ){
		/* we have a number -- use atoi to convert */
		stringstream ss;
		while ( is_char_number(*ch) ){
			ss << (*ch);
			ichar++;
			ch++;	
		}
		ichar --;
		fobj->type = E_FML_NUMBER;
		fobj->data.num = my_atoi(ss.str().c_str());
	} else {
		switch ((*ch)){
			case 'W':
				fobj->type = E_FML_NUMBER;
				fobj->data.num = mydata.dest_W;
				break;
			case 't':
				fobj->type = E_FML_NUMBER;
				fobj->data.num = mydata.track;
				break;
			case '+':
				fobj->type = E_FML_OPERATOR;
				fobj->data.op = E_OP_ADD;
				break;
			case '-':
				fobj->type = E_FML_OPERATOR;
				fobj->data.op = E_OP_SUB;
				break;
			case '/':
				fobj->type = E_FML_OPERATOR;
				fobj->data.op = E_OP_DIV;
				break;
			case '*':
				fobj->type = E_FML_OPERATOR;
				fobj->data.op = E_OP_MULT;
				break;
			case '(':
				fobj->type = E_FML_BRACKET;
				fobj->data.left_bracket = true;
				break;
			case ')':
				fobj->type = E_FML_BRACKET;
				fobj->data.left_bracket = false;
				break;
			default:
				vpr_printf(TIO_MESSAGE_ERROR, "in get_formula_object: unsupported character: '%c'\n", ch);
				break; 
		}	
	}
	
	return;
}

/* returns integer specifying precedence of passed-in operator. higher integer 
   means higher precedence */
static int get_fobj_precedence( INP const Formula_Object &fobj ){
	int precedence = 0;

	if (E_FML_BRACKET == fobj.type){
		precedence = 0;
	} else if (E_FML_OPERATOR == fobj.type){
		t_operator op = fobj.data.op;
		switch (op){
			case E_OP_ADD: 
				precedence = 2;
				break;
			case E_OP_SUB: 
				precedence = 2;
				break;
			case E_OP_MULT: 
				precedence = 3;
				break;
			case E_OP_DIV: 
				precedence = 3;
				break;
			default:
				vpr_printf(TIO_MESSAGE_ERROR, "in get_fobj_precedence: unrecognized operator: %d\n", op);
				exit(1);
				break; 
		}
	} else {
		vpr_printf(TIO_MESSAGE_ERROR, "in get_fobj_precedence: no precedence possible for formula object type %d\n", fobj.type);
		exit(1);
	}
	
	return precedence;
}

/* Returns associativity of the specified operator */
static bool op_associativity_is_left( INP const t_operator &op ){
	bool is_left = true;
	
	/* associativity is 'left' for all but the power operator, which is not yet implemented */
	//TODO:
	//if op is 'power' set associativity is_left=false and return

	return is_left;
}

/* used by the shunting-yard formula parser to deal with operators such as add and subtract */
static void handle_operator( INP const Formula_Object &fobj, INOUTP vector<Formula_Object> &rpn_output,
				INOUTP stack<Formula_Object> &op_stack){
	if ( E_FML_OPERATOR != fobj.type){
		vpr_printf(TIO_MESSAGE_ERROR, "in handle_operator: passed in formula object not of type operator\n");
		exit(1);
	}

	int op_pr = get_fobj_precedence( fobj );
	bool op_assoc_is_left = op_associativity_is_left( fobj.data.op );

	Formula_Object fobj_dummy;
	bool keep_going = false;
	do{
		/* here we keep popping operators off the stack onto back of rpn_output while
		   associativity of operator is 'left' and precedence op_pr = top_pr, or while
		   precedence op_pr < top_pr */

		/* determine whether we should keep popping operators off the op stack */
		if ( op_stack.empty() ){
			keep_going = false;
		} else {
			/* get precedence of top operator */
			int top_pr = get_fobj_precedence ( op_stack.top() );

			keep_going = ( (op_assoc_is_left && op_pr==top_pr)
					|| op_pr<top_pr );
			
			if (keep_going){
				/* pop top operator off stack onto the back of rpn_output */
				fobj_dummy = op_stack.top();
				rpn_output.push_back( fobj_dummy );
				op_stack.pop();
			}
		}

	} while (keep_going);

	/* place new operator object on top of stack */
	op_stack.push(fobj);
	
	return;
}

/* used by the shunting-yard formula parser to deal with brackets, ie '(' and ')' */
static void handle_bracket( INP const Formula_Object &fobj, INOUTP vector<Formula_Object> &rpn_output,
				INOUTP stack<Formula_Object> &op_stack ){
	if ( E_FML_BRACKET != fobj.type){
		vpr_printf(TIO_MESSAGE_ERROR, "in handle_bracket: passed-in formula object not of type bracket\n");
		exit(1);
	}

	/* check if left or right bracket */
	if ( fobj.data.left_bracket ){
		/* left bracket, so simply push it onto operator stack */
		op_stack.push(fobj);
	} else {
		bool keep_going = false;
		do{
			/* here we keep popping operators off op_stack onto back of rpn_output until a 
			   left bracket is encountered */

			if ( op_stack.empty() ){
				/* didn't find an opening bracket - mismatched brackets */
				vpr_printf(TIO_MESSAGE_ERROR, "Ran out of stack while parsing brackets -- bracket mismatch in user-specified formula\n");
				exit(1); 
				keep_going = false;
			}
	
			Formula_Object next_fobj = op_stack.top();
			if (E_FML_BRACKET == next_fobj.type){
				if (next_fobj.data.left_bracket){
					/* matching bracket found -- pop off stack and finish */
					op_stack.pop();
					keep_going = false;
				} else {
					/* should not find two right brackets without a left bracket in-between */
					vpr_printf(TIO_MESSAGE_ERROR, "Mismatched brackets encountered in user-specified formula\n");
					exit(1); 
					keep_going = false;
				}
			} else if (E_FML_OPERATOR == next_fobj.type){
				/* pop operator off stack onto the back of rpn_output */
				Formula_Object fobj_dummy = op_stack.top();
				rpn_output.push_back( fobj_dummy );
				op_stack.pop();
				keep_going = true;
			} else {
				vpr_printf(TIO_MESSAGE_ERROR, "Found unexpected formula object on operator stack: %d\n", next_fobj.type);
				exit(1);
				keep_going = false;
			}
		} while (keep_going);
	}
	return;
}


/* parses a reverse-polish notation vector corresponding to a switchblock formula
   and returns the integer result */
static int parse_rpn_vector( INOUTP vector<Formula_Object> &rpn_vec ){
	int result = -1;

	/* first entry should always be a number */
	if (E_FML_NUMBER != rpn_vec.at(0).type){
		vpr_printf(TIO_MESSAGE_ERROR, "parse_rpn_vector: first entry is not a number\n");
		exit(1);
	}

	if (rpn_vec.size() == 1){
		/* if the vector size is 1 then we just have a number (which was verified above) */
		result = rpn_vec.at(0).data.num;
	} else {
		/* have numbers and operators */
		Formula_Object fobj;
		int ivec = 0;
		/* keep going until we have gone through the whole vector */
		while ( !rpn_vec.empty() ){
			
			/* keep going until we have hit an operator */
			do{
				ivec++;		/* first item should never be operator anyway */
				if (ivec == (int)rpn_vec.size()){
					vpr_printf(TIO_MESSAGE_ERROR, "parse_rpn_vector(): found multiple numbers in switchblock formula, but no operator\n");
					exit(1);
				}
			} while ( E_FML_OPERATOR != rpn_vec.at(ivec).type );

			/* now we apply the selected operation to the two previous entries */
			/* the result is stored in the object that used to be the operation */
			rpn_vec.at(ivec).data.num = apply_rpn_op( rpn_vec.at(ivec-2), rpn_vec.at(ivec-1), rpn_vec.at(ivec) );
			rpn_vec.at(ivec).type = E_FML_NUMBER;

			/* remove the previous two entries from the vector */
			rpn_vec.erase(rpn_vec.begin() + ivec - 2, rpn_vec.begin() + ivec - 0);
			ivec -= 2;

			/* if we're down to one element, we are done */
			if (1 == rpn_vec.size()){
				result = rpn_vec.at(ivec).data.num;
				rpn_vec.erase(rpn_vec.begin() + ivec);
			}
		}
	}

	return result;
}

/* applies operation specified by 'op' to the given arguments. arg1 comes before arg2 */
static int apply_rpn_op( INP const Formula_Object &arg1, INP const Formula_Object &arg2, 
					INP const Formula_Object &op ){
	int result = -1;
	
	/* arguments must be numbers */
	if ( E_FML_NUMBER != arg1.type || 
	     E_FML_NUMBER != arg2.type){
		vpr_printf(TIO_MESSAGE_ERROR, "in apply_rpn_op: one of the arguments is not a number\n");
		exit(1);
	}

	/* check that op is actually an operation */
	if ( E_FML_OPERATOR != op.type ){
		vpr_printf(TIO_MESSAGE_ERROR, "in apply_rpn_op: the object specified as the operation is not of operation type\n");
		exit(1);
	}

	/* apply operation to arguments */
	switch (op.data.op){
		case E_OP_ADD:
			result = arg1.data.num + arg2.data.num;
			break;
		case E_OP_SUB:
			result = arg1.data.num - arg2.data.num;
			break;
		case E_OP_MULT:
			result = arg1.data.num * arg2.data.num;
			break;
		case E_OP_DIV:
			result = arg1.data.num / arg2.data.num;
			break;
		default:
			vpr_printf(TIO_MESSAGE_ERROR, "in apply_rpn_op: invalid operation: %d\n", op.data.op);
			exit(1);
			break;
	}
	
	return result;
} 


/* checks if specified character represents an ASCII number */
static bool is_char_number ( INP const char ch ){
	bool result = false;
	
	if ( ch >= '0' && ch <= '9' ){
		result = true;
	} else {
		result = false;
	}

	return result;
}

/* checks if the specified formula is piece-wise defined */
static bool is_piecewise_formula( INP const char *formula ){
	bool result = false;
	/* if formula is piecewise, we expect '{' to be the very first character */
	if ('{' == formula[0]){
		result = true;
	} else {
		result = false;
	}
	return result;
}
