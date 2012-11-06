/*********************************************************************
 *  The following code is part of the power modelling feature of VTR.
 *
 * For support:
 * http://code.google.com/p/vtr-verilog-to-routing/wiki/Power
 *
 * or email:
 * vtr.power.estimation@gmail.com
 *
 * If you are using power estimation for your researach please cite:
 *
 * Jeffrey Goeders and Steven Wilton.  VersaPower: Power Estimation
 * for Diverse FPGA Architectures.  In International Conference on
 * Field Programmable Technology, 2012.
 *
 ********************************************************************/

/**
 * This file provides functions that calculate the power of low-level
 * components (inverters, simple multiplexers, etc)
 */

/************************* INCLUDES *********************************/
#include <assert.h>

#include "power_lowlevel.h"
#include "power_util.h"
#include "power_cmos_tech.h"
#include "globals.h"

/************************* FUNCTION DELCARATIONS ********************/
static float power_calc_node_switching_v(float capacitance, float density,
		float voltage);
static void power_calc_transistor_capacitance(float *C_d, float *C_s,
		float *C_g, e_tx_type transistor_type, float size);
static float power_calc_leakage_st(e_tx_type transistor_type,
		float size);
static float power_calc_leakage_st_pass_transistor(float size, float v_ds);
static float power_calc_leakage_gate(e_tx_type transistor_type, float size);
static float power_calc_buffer_sc_levr(
		t_power_buffer_strength_inf * buffer_strength, int input_mux_size);

/************************* FUNCTION DEFINITIONS *********************/

/**
 * Initializer function for this module, called by power_init
 */
void power_lowlevel_init() {
	float C_d, C_s, C_g;

	power_calc_transistor_capacitance(&C_d, &C_s, &C_g, NMOS, 1.0);
	g_power_commonly_used->NMOS_1X_C_d = C_d;
	g_power_commonly_used->NMOS_1X_C_g = C_g;
	g_power_commonly_used->NMOS_1X_C_s = C_s;

	power_calc_transistor_capacitance(&C_d, &C_s, &C_g, PMOS,
			g_power_tech->PN_ratio);
	g_power_commonly_used->PMOS_1X_C_d = C_d;
	g_power_commonly_used->PMOS_1X_C_g = C_g;
	g_power_commonly_used->PMOS_1X_C_s = C_s;

	g_power_commonly_used->NMOS_1X_st_leakage = power_calc_leakage_st(
			NMOS, 1.0);
	g_power_commonly_used->PMOS_1X_st_leakage = power_calc_leakage_st(
			PMOS, 1.0 * g_power_tech->PN_ratio);

	g_power_commonly_used->INV_1X_C_in = g_power_commonly_used->NMOS_1X_C_g
			+ g_power_commonly_used->PMOS_1X_C_g;
	g_power_commonly_used->INV_1X_C = g_power_commonly_used->NMOS_1X_C_g
			+ g_power_commonly_used->PMOS_1X_C_g
			+ g_power_commonly_used->NMOS_1X_C_d
			+ g_power_commonly_used->PMOS_1X_C_d;

	power_calc_transistor_capacitance(&C_d, &C_s, &C_g, NMOS, 2.0);
	g_power_commonly_used->INV_2X_C = C_g + C_d;
	power_calc_transistor_capacitance(&C_d, &C_s, &C_g, PMOS,
			2.0 * g_power_tech->PN_ratio);
	g_power_commonly_used->INV_2X_C += C_g + C_d;

}

/**
 * Calculates the switching power of a node
 * - capacitance: The capacitance of the nodoe
 * - density: The transition density of the node
 */
float power_calc_node_switching(float capacitance, float density) {
	return 0.5 * g_power_tech->Vdd * g_power_tech->Vdd * capacitance * density
			/ g_solution_inf.T_crit;
}

/**
 * Calculates the switching power of a node, with non-Vdd voltage
 * - capacitance: The capacitance of the nodoe
 * - density: The transition density of the node
 * - voltage: The voltage when the node is charged
 */
static float power_calc_node_switching_v(float capacitance, float density,
		float voltage) {
	return 0.5 * voltage * g_power_tech->Vdd * capacitance * density
			/ g_solution_inf.T_crit;
}

/**
 * Calculates the power of an inverter
 * - power_usage: (Return value) The power usage of the inverter
 * - in_dens: The transition density of the input
 * - in_prob: The signal probability of the input
 * - size: The inverter size, relative to a min-size inverter
 */
void power_calc_inverter(t_power_usage * power_usage, float in_dens,
		float in_prob, float size) {
	float C_drain, C_gate, C_source;
	float C_inv;

	float PMOS_size = g_power_tech->PN_ratio * size;
	float NMOS_size = size;

	power_usage->dynamic = 0.;
	power_usage->leakage = 0.;

	C_inv = 0.;

	power_calc_transistor_capacitance(&C_drain, &C_source, &C_gate, NMOS,
			NMOS_size);
	C_inv += C_gate + C_drain;

	power_calc_transistor_capacitance(&C_drain, &C_source, &C_gate, PMOS,
			PMOS_size);
	C_inv += C_gate + C_drain;

	power_usage->dynamic = power_calc_node_switching(C_inv, in_dens);

	power_usage->leakage = in_prob
			* power_calc_leakage_st(PMOS, PMOS_size)
			+ (1 - in_prob) * power_calc_leakage_st(NMOS, NMOS_size);

	power_usage->leakage += in_prob * power_calc_leakage_gate(NMOS, NMOS_size)
			+ (1 - in_prob) * power_calc_leakage_gate(PMOS, PMOS_size);
}

/**
 * Calculates the power of an inverter, with irregular P/N ratio
 * - power_usage: (Return value) The power usage of the inverter
 * - dy_power_input: (Return value) The dynamic power of the input node
 * - in_dens: The transition density of the input
 * - in_prob: The signal probability of the input
 * - PMOS_size: (W/L) of the PMOS
 * - NMOS_size: (W/L) of the NMOS
 */
void power_calc_inverter_irregular(t_power_usage * power_usage,
		float * dyn_power_input, float in_density, float in_probability,
		float PMOS_size, float NMOS_size) {
	float C_drain, C_gate, C_source;
	float C_inv;
	float C_in;

	power_usage->dynamic = 0.;
	power_usage->leakage = 0.;

	C_inv = 0.;
	C_in = 0.;

	power_calc_transistor_capacitance(&C_drain, &C_source, &C_gate, NMOS,
			NMOS_size);
	C_inv += C_gate + C_drain;
	C_in += C_gate;

	power_calc_transistor_capacitance(&C_drain, &C_source, &C_gate, PMOS,
			PMOS_size);
	C_inv += C_gate + C_drain;
	C_in += C_gate;

	power_usage->dynamic = power_calc_node_switching(C_inv, in_density);
	*dyn_power_input = power_calc_node_switching(C_in, in_density);

	power_usage->leakage = in_probability
			* power_calc_leakage_st(PMOS, PMOS_size)
			+ (1 - in_probability)
					* power_calc_leakage_st(NMOS, NMOS_size);
}

/**
 * Calculates the power of an inverter, also returning dynamic power of the input
 * - power_usage: (Return value) The power usage of the inverter
 * - input_dynamic_power: (Return value) The dynamic power of the input node
 * - in_dens: The transition density of the input
 * - in_prob: The signal probability of the input
 * - size: The inverter size, relative to a min-size inverter
 */
void power_calc_inverter_with_input(t_power_usage * power_usage,
		float * input_dynamic_power, float in_density, float in_prob,
		float size) {
	float C_drain, C_gate, C_source;
	float C_inv;
	float C_in;

	float PMOS_size = g_power_tech->PN_ratio * size;
	float NMOS_size = size;

	power_usage->dynamic = 0.;
	power_usage->leakage = 0.;

	C_inv = 0.;
	C_in = 0.;

	power_calc_transistor_capacitance(&C_drain, &C_source, &C_gate, NMOS,
			NMOS_size);
	C_inv += C_gate + C_drain;
	C_in += C_gate;

	power_calc_transistor_capacitance(&C_drain, &C_source, &C_gate, PMOS,
			PMOS_size);
	C_inv += C_gate + C_drain;
	C_in += C_gate;

	power_usage->dynamic = power_calc_node_switching(C_inv, in_density);
	*input_dynamic_power = power_calc_node_switching(C_in, in_density);

	power_usage->leakage = in_prob
			* power_calc_leakage_st(PMOS, PMOS_size)
			+ (1 - in_prob) * power_calc_leakage_st(NMOS, NMOS_size);

	power_usage->leakage += in_prob * power_calc_leakage_gate(NMOS, NMOS_size)
			+ (1 - in_prob) * power_calc_leakage_gate(PMOS, PMOS_size);
}

/**
 * Calculate the capacitance for a transistor
 * - C_d: (Return value) Drain capacitance
 * - C_s: (Return value) Source capacitance
 * - C_g: (Return value) Gate capacitance
 * - transistor_type: NMOS or PMOS
 * - size: (W/L) size of the transistor
 */
static void power_calc_transistor_capacitance(float *C_d, float *C_s,
		float *C_g, e_tx_type transistor_type, float size) {
	t_transistor_size_inf * tx_info_lower;
	t_transistor_size_inf * tx_info_upper;
	boolean error;

	/* Initialize to 0 */
	*C_d = 0.;
	*C_s = 0.;
	*C_g = 0.;

	error = power_find_transistor_info(&tx_info_lower, &tx_info_upper,
			transistor_type, size);
	if (error) {
		return;
	}

	if (tx_info_lower == NULL ) {
		/* No lower bound */
		*C_d = tx_info_upper->C_d;
		*C_s = tx_info_upper->C_s;
		*C_g = tx_info_upper->C_g;
	} else if (tx_info_upper == NULL ) {
		/* No upper bound */
		*C_d = tx_info_lower->C_d;
		*C_s = tx_info_lower->C_s;
		*C_g = tx_info_lower->C_g;
	} else {
		/* Linear approximation between sizes */
		float percent_upper = (size - tx_info_lower->size)
				/ (tx_info_upper->size - tx_info_lower->size);
		*C_d = (1 - percent_upper) * tx_info_lower->C_d
				+ percent_upper * tx_info_upper->C_d;
		*C_s = (1 - percent_upper) * tx_info_lower->C_s
				+ percent_upper * tx_info_upper->C_s;
		*C_g = (1 - percent_upper) * tx_info_lower->C_g
				+ percent_upper * tx_info_upper->C_g;
	}

	return;
}

/**
 * Returns the subthreshold leakage power of a transistor,
 * for V_ds = V_dd
 * - transistor_type: NMOS or PMOS
 * - size: (W/L) of transistor
 */
static float power_calc_leakage_st(e_tx_type transistor_type,
		float size) {
	t_transistor_size_inf * tx_info_lower;
	t_transistor_size_inf * tx_info_upper;
	boolean error;
	float current;

	error = power_find_transistor_info(&tx_info_lower, &tx_info_upper,
			transistor_type, size);
	if (error) {
		return 0;
	}

	if (tx_info_lower == NULL ) {
		/* No lower bound */
		current = tx_info_upper->leakage_subthreshold;

	} else if (tx_info_upper == NULL ) {
		/* No upper bound */
		current = tx_info_lower->leakage_subthreshold;
	} else {
		/* Linear approximation between sizes */
		float percent_upper = (size - tx_info_lower->size)
				/ (tx_info_upper->size - tx_info_lower->size);
		current = (1 - percent_upper) * tx_info_lower->leakage_subthreshold
				+ percent_upper * tx_info_upper->leakage_subthreshold;
	}

	return current * g_power_tech->Vdd;
}

/**
 * Returns the gate gate leakage power of a transistor
 * - transistor_type: NMOS or PMOS
 * - size: (W/L) of transistor
 */
static float power_calc_leakage_gate(e_tx_type transistor_type, float size) {
	t_transistor_size_inf * tx_info_lower;
	t_transistor_size_inf * tx_info_upper;
	boolean error;
	float current;

	error = power_find_transistor_info(&tx_info_lower, &tx_info_upper,
			transistor_type, size);
	if (error) {
		return 0;
	}

	if (tx_info_lower == NULL ) {
		/* No lower bound */
		current = tx_info_upper->leakage_gate;

	} else if (tx_info_upper == NULL ) {
		/* No upper bound */
		current = tx_info_lower->leakage_gate;
	} else {
		/* Linear approximation between sizes */
		float percent_upper = (size - tx_info_lower->size)
				/ (tx_info_upper->size - tx_info_lower->size);
		current = (1 - percent_upper) * tx_info_lower->leakage_gate
				+ percent_upper * tx_info_upper->leakage_gate;
	}

	return current * g_power_tech->Vdd;
}

/**
 * Returns the subthreshold leakage power of a pass-transistor,
 * assumed to be a minimum-sized NMOS
 * - size: (W/L) size of transistor (Must be 1.0)
 * - v_ds: Drain-source voltage
 */
static float power_calc_leakage_st_pass_transistor(float size, float v_ds) {
	t_power_nmos_leakage_pair * lower;
	t_power_nmos_leakage_pair * upper;
	float i_ds;

	assert(size == 1.0);

	power_find_nmos_leakage(&lower, &upper, v_ds);

	if (lower->v_ds == v_ds || !upper) {
		i_ds = lower->i_ds;
	} else {
		float perc_upper = (v_ds - lower->v_ds) / (upper->v_ds - lower->v_ds);
		i_ds = (1 - perc_upper) * lower->i_ds + perc_upper * upper->i_ds;
	}

	return i_ds * g_power_tech->Vdd;
}

/**
 * Calculates the power of a wire
 * - power_usage: (Return value) Power usage of the wire
 * - capacitance: Capacitance of the wire (F)
 * - density: Transition density of the wire
 */
void power_usage_wire(t_power_usage * power_usage, float capacitance,
		float density) {
	power_usage->leakage = 0.;
	power_usage->dynamic = power_calc_node_switching(capacitance, density);
}

/**
 * Calculates the power of a 2-input multiplexer, comprised of transmission gates
 * - power_usage: (Return value) Power usage of the mux
 * - in_dens: Array of input transition densities
 * - in_prob: Array of input signal probabilities
 * - sel_desn: Transition density of select line
 * - sel_prob: Signal probability of select line
 * - out_dens: Transition density of the output
 */
void power_calc_MUX2_transmission(t_power_usage * power_usage, float * in_dens,
		float * in_prob, float sel_dens, float sel_prob, float out_dens) {

	power_zero_usage(power_usage);

	/* A transmission gate leaks if the selected input != other input  */
	power_usage->leakage += (in_prob[0] * (1 - in_prob[1])
			+ (1 - in_prob[0]) * in_prob[1])
			* (g_power_commonly_used->NMOS_1X_st_leakage
					+ g_power_commonly_used->PMOS_1X_st_leakage);

	/* Gate switching */
	power_usage->dynamic += 2
			* power_calc_node_switching(
					g_power_commonly_used->NMOS_1X_C_g
							+ g_power_commonly_used->PMOS_1X_C_g, sel_dens);

	/* Input switching */
	power_usage->dynamic += power_calc_node_switching(
			g_power_commonly_used->NMOS_1X_C_d
					+ g_power_commonly_used->PMOS_1X_C_s, in_dens[0]);
	power_usage->dynamic += power_calc_node_switching(
			g_power_commonly_used->NMOS_1X_C_d
					+ g_power_commonly_used->PMOS_1X_C_s, in_dens[1]);

	/* Output switching */
	power_usage->dynamic += power_calc_node_switching(
			2
					* (g_power_commonly_used->NMOS_1X_C_s
							+ g_power_commonly_used->PMOS_1X_C_d), out_dens);
}

/**
 * Calucates the power of a static, single-level multiplexer
 * - power_usage: (Return value) power usage of the mux
 * - out_prob: (Return value) Signal probability of the output
 * - out_dens: (Return value) Transition density of the output
 * - num_inputs: Number of inputs of the mux
 * - selected_idx: The input index that is selected by the select lines
 * - in_prob: Array of input signal probabilities
 * - in_dens: Array of input tranistion densities
 * - v_in: Array of input max voltages
 * - transistor_size: Size of the NMOS transistors (must be 1.0)
 * - v_out_restored: Whether the output will be level restored to Vdd
 */
void power_calc_mux_singlelevel_static(t_power_usage * power_usage,
		float * out_prob, float * out_dens, float * v_out, int num_inputs,
		int selected_idx, float * in_prob, float * in_dens, float * v_in,
		float transistor_size, boolean v_out_restored) {
	int input_idx;
	float v_in_selected;
	float in_prob_avg;

	power_zero_usage(power_usage);

	assert(transistor_size == 1.0);

	if (selected_idx < num_inputs) {
		*out_prob = in_prob[selected_idx];
		*out_dens = in_dens[selected_idx];
		v_in_selected = v_in[selected_idx];

	} else {
		/* In this case, the multiplexer is not symetrical.  The
		 * other branch of the mux has more inputs than this one,
		 * and the selected input index is not a valid index for
		 * this portion of the mux.  If the mux was actually built
		 * this way, there would likely be a weak pull-up to ensure
		 * that the node does not float.
		 */
		*out_prob = 1.0;
		*out_dens = 0.0;

		v_in_selected = 0.;
		for (input_idx = 0; input_idx < num_inputs; input_idx++) {
			v_in_selected += v_in[input_idx];
		}
		v_in_selected /= num_inputs;
	}

	in_prob_avg = 0.;
	for (input_idx = 0; input_idx < num_inputs; input_idx++) {
		/* Dynamic Power at Inputs */
		power_usage->dynamic += power_calc_node_switching_v(
				g_power_commonly_used->NMOS_1X_C_d, in_dens[input_idx],
				v_in[input_idx]);

		if (input_idx != selected_idx) {
			in_prob_avg += in_prob[input_idx];
		}
	}
	in_prob_avg /= (num_inputs - 1);

	if (v_out_restored) {
		*v_out = g_power_tech->Vdd;
	} else {
		*v_out = power_calc_mux_v_out(num_inputs, transistor_size,
				v_in_selected, in_prob_avg);
	}

	for (input_idx = 0; input_idx < num_inputs; input_idx++) {
		/* Leakage */
		/* The selected input will never leak */
		if (input_idx == selected_idx) {
			continue;
		}

		/* Output is high and this input is low */
		power_usage->leakage += (*out_prob) * (1 - in_prob[input_idx])
				* power_calc_leakage_st_pass_transistor(transistor_size, *v_out);

		/* Output is low and this input is high */
		power_usage->leakage += (1 - *out_prob) * in_prob[input_idx]
				* power_calc_leakage_st_pass_transistor(transistor_size,
						v_in[input_idx]);
	}

	/* Dynamic Power at Output */
	power_usage->dynamic += power_calc_node_switching_v(
			g_power_commonly_used->NMOS_1X_C_s * num_inputs, *out_dens, *v_out);

}

/**
 * This function calcualtes the output voltage of a single-level multiplexer
 * - num_inputs: Number of inputs of the multiplexer
 * - transistor_size: The size of the NMOS transistors (must be 1.0)
 * - v_in: The input voltage of the selcted input
 * - in_prob_avg: The average signal probabilities of the non-selected inputs
 */
float power_calc_mux_v_out(int num_inputs, float transistor_size, float v_in,
		float in_prob_avg) {
	t_power_mux_volt_inf * mux_volt_inf;
	t_power_mux_volt_pair * lower;
	t_power_mux_volt_pair * upper;
	float v_out_min, v_out_max;

	assert(transistor_size = 1.0);

	if (num_inputs > g_power_tech->max_mux_sl_size) {
		power_log_msg(POWER_LOG_ERROR,
				"The circuit contains a single-level mux larger than \
				what is defined in the <multiplexers> section of the \
				technology file.");
	}

	mux_volt_inf = &g_power_tech->mux_voltage_inf[num_inputs];

	power_find_mux_volt_inf(&lower, &upper, mux_volt_inf, v_in);

	if (lower->v_in == v_in || !upper) {
		v_out_min = lower->v_out_min;
		v_out_max = lower->v_out_max;
	} else {
		float perc_upper = (v_in - lower->v_in) / (upper->v_in - lower->v_in);
		v_out_min = (1 - perc_upper) * lower->v_out_min
				+ perc_upper * upper->v_out_min;
		v_out_max = (1 - perc_upper) * lower->v_out_max
				+ perc_upper * upper->v_out_max;
	}

	return in_prob_avg * v_out_max + (1 - in_prob_avg) * v_out_min;
}

/** This function calculates the power of a single-level multiplexer, where the
 * select lines are dynamic
 * - power_usage: (Return value) The power usage of the mux
 * - num_inputs: Number of multiplexer inputs (must be 2)
 * - out_density: The transition density of the output
 * - out_prob: The signal probability of the output
 * - v_out: The output max voltage
 * - in_prob: Array of input signal probabilities
 * - in_dens: Array of input tranistion densities
 * - v_in: Array of input voltages
 * - sel_dens: Transition density of the select line
 * - sel_prob: Signal probability of the select line
 * - tranisistor_size: NMOS transistor sizes (must be 1.0)
 */
void power_calc_mux_singlelevel_dynamic(t_power_usage * power_usage,
		int num_inputs, float out_density, float out_prob, float v_out,
		float * in_prob, float * in_dens, float * v_in, float sel_dens,
		float sel_prob, float transistor_size) {

	assert(num_inputs == 2);
	assert(transistor_size = 1.0);

	power_zero_usage(power_usage);

	/* Leakage occurs when input1 != input2.
	 * If the selected input is low, the other transistor leaks input->output
	 * If the selected input is high, the other transistor leaks output->input*/

	/* 1st selected, 1st Low, 2nd High - Leakage from 2nd in->out */
	power_usage->leakage += (1 - sel_prob) * (1 - in_prob[0]) * in_prob[1]
			* power_calc_leakage_st_pass_transistor(1.0, v_in[1]);

	/* 1st selected, 1st High, 2nd Low - Leakage from 2nd out->in */
	/* 2nd selected, 1st Low, 2nd High - Leakage from 1st out->in */
	power_usage->leakage += ((1 - sel_prob) * in_prob[0] * (1 - in_prob[1])
			+ sel_prob * (1 - in_prob[0]) * in_prob[1])
			* power_calc_leakage_st_pass_transistor(1.0, v_out);

	/* 2nd selected, 1st High, 2nd Low - Leakage from 1st in->out */
	power_usage->leakage += sel_prob * in_prob[0] * (1 - in_prob[1])
			* power_calc_leakage_st_pass_transistor(1.0, v_in[0]);

	/* Gate switching */
	power_usage->dynamic += 2
			* power_calc_node_switching(g_power_commonly_used->NMOS_1X_C_g,
					sel_dens);

	/* Input switching */
	power_usage->dynamic += power_calc_node_switching_v(
			g_power_commonly_used->NMOS_1X_C_d, in_dens[0], v_in[0]);
	power_usage->dynamic += power_calc_node_switching_v(
			g_power_commonly_used->NMOS_1X_C_d, in_dens[1], v_in[1]);

	/* Output switching */
	power_usage->dynamic += power_calc_node_switching_v(
			2 * g_power_commonly_used->NMOS_1X_C_s, out_density, v_out);
}

/**
 * This function calculates the power of a level restorer, which is a biased
 * inverter with a pull-up PMOS transistor in feedback.
 * - power_usage: (Return value) Power usage of the level restorer
 * - dyn_power_in: (Return value) Dynamic power at the input
 * - in_density: Transition density of the input
 * - in_prob: Signal probability of the input
 */
void power_calc_level_restorer(t_power_usage * power_usage,
		float * dyn_power_in, float in_dens, float in_prob) {
	t_power_usage sub_power_usage;
	float C;
	float C_in;
	float input_dyn_power = 0.;

	power_zero_usage(power_usage);

	/* Inverter */
	power_calc_inverter_irregular(&sub_power_usage, &input_dyn_power,
			in_dens, in_prob, 1.0, 2.0);
	power_add_usage(power_usage, &sub_power_usage);

	/* Pull-up PMOS */
	if (g_power_tech->PMOS_inf.long_trans_inf == NULL ) {
		power_log_msg(POWER_LOG_ERROR,
				"No long transistor information exists.  Cannot determine transistor properties.");
		return;
	}
	C = g_power_tech->PMOS_inf.long_trans_inf->C_d
			+ g_power_tech->PMOS_inf.long_trans_inf->C_g;
	C_in = g_power_tech->PMOS_inf.long_trans_inf->C_d;

	input_dyn_power += power_calc_node_switching(C_in, in_dens);
	power_usage->dynamic += power_calc_node_switching(C, in_dens);
	power_usage->leakage += (1 - in_prob)
			* g_power_tech->PMOS_inf.long_trans_inf->leakage_subthreshold;

	*dyn_power_in = input_dyn_power;
}

/**
 *  This function calculates the short-circuit factor for a buffer.  This factor
 *  represents the short-circuit power of a buffer, as a factor of switching power.
 *  - stages: Number of stages of the buffer
 *  - gain: The gain at each stage
 *  - level_restorer: Whether this buffer must level-restore the input to Vdd
 *  - input_mux_size: For level-restoring buffers, what is the size of the mux driving it
 */
float power_calc_buffer_sc(int stages, float gain, boolean level_restorer,
		int input_mux_size) {

	t_power_buffer_size_inf * size_inf;
	t_power_buffer_strength_inf * strength_lower;
	t_power_buffer_strength_inf * strength_upper;
	float sc;

	/* Find information for given buffer size */
	size_inf = &g_power_tech->buffer_size_inf[stages];

	/* Find information for a given size/strength */
	power_find_buffer_strength_inf(&strength_lower, &strength_upper, size_inf,
			gain);

	if (!level_restorer) {
		if (strength_upper == NULL ) {
			sc = strength_lower->sc_no_levr;
		} else {
			float percent_upper = (gain - strength_lower->stage_gain)
					/ (strength_upper->stage_gain - strength_lower->stage_gain);
			sc = (1 - percent_upper) * strength_lower->sc_no_levr
					+ percent_upper * strength_upper->sc_no_levr;
		}
	} else {
		/* Level Restored - Short Circuit depends on input mux size */

		if (strength_upper == NULL ) {
			sc = power_calc_buffer_sc_levr(strength_lower, input_mux_size);
		} else {
			float sc_buf_low;
			float sc_buf_high;

			sc_buf_low = power_calc_buffer_sc_levr(strength_lower,
					input_mux_size);
			sc_buf_high = power_calc_buffer_sc_levr(strength_upper,
					input_mux_size);

			float percent_upper = (gain - strength_lower->stage_gain)
					/ (strength_upper->stage_gain - strength_lower->stage_gain);
			sc = (1 - percent_upper) * sc_buf_low + percent_upper * sc_buf_high;
		}
	}
	return sc;
}

/**
 * This function calculates the short-circuit factor for a level-restoring buffer,
 * used by power_calc_buffer_sc
 * - buffer_strength: The buffer information, for a given size/strength
 * - input_mux_size: The size of the mux driving this buffer
 */
static float power_calc_buffer_sc_levr(
		t_power_buffer_strength_inf * buffer_strength, int input_mux_size) {
	t_power_buffer_sc_levr_inf * mux_lower;
	t_power_buffer_sc_levr_inf * mux_upper;

	power_find_buffer_sc_levr(&mux_lower, &mux_upper, buffer_strength,
			input_mux_size);
	if (mux_upper == NULL ) {
		return mux_lower->sc_levr;
	} else {
		float percent_upper = (input_mux_size - mux_lower->mux_size)
				/ (mux_upper->mux_size - mux_lower->mux_size);
		return (1 - percent_upper) * mux_lower->sc_levr
				+ percent_upper * mux_upper->sc_levr;
	}
}
