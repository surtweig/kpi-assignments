//lpm_shiftreg CBX_SINGLE_OUTPUT_FILE="ON" LPM_DIRECTION="RIGHT" LPM_TYPE="LPM_SHIFTREG" LPM_WIDTH=8 aclr aset clock data load q
//VERSION_BEGIN 19.1 cbx_mgl 2019:09:22:09:26:20:SJ cbx_stratixii 2019:09:22:08:02:34:SJ cbx_util_mgl 2019:09:22:08:02:34:SJ  VERSION_END
// synthesis VERILOG_INPUT_VERSION VERILOG_2001
// altera message_off 10463



// Copyright (C) 2019  Intel Corporation. All rights reserved.
//  Your use of Intel Corporation's design tools, logic functions 
//  and other software and tools, and any partner logic 
//  functions, and any output files from any of the foregoing 
//  (including device programming or simulation files), and any 
//  associated documentation or information are expressly subject 
//  to the terms and conditions of the Intel Program License 
//  Subscription Agreement, the Intel Quartus Prime License Agreement,
//  the Intel FPGA IP License Agreement, or other applicable license
//  agreement, including, without limitation, that your use is for
//  the sole purpose of programming logic devices manufactured by
//  Intel and sold by Intel or its authorized distributors.  Please
//  refer to the applicable agreement for further details, at
//  https://fpgasoftware.intel.com/eula.



//synthesis_resources = lpm_shiftreg 1 
//synopsys translate_off
`timescale 1 ps / 1 ps
//synopsys translate_on
module  mg9ob
	( 
	aclr,
	aset,
	clock,
	data,
	load,
	q) /* synthesis synthesis_clearbox=1 */;
	input   aclr;
	input   aset;
	input   clock;
	input   [7:0]  data;
	input   load;
	output   [7:0]  q;

	wire  [7:0]   wire_mgl_prim1_q;

	lpm_shiftreg   mgl_prim1
	( 
	.aclr(aclr),
	.aset(aset),
	.clock(clock),
	.data(data),
	.load(load),
	.q(wire_mgl_prim1_q));
	defparam
		mgl_prim1.lpm_direction = "RIGHT",
		mgl_prim1.lpm_type = "LPM_SHIFTREG",
		mgl_prim1.lpm_width = 8;
	assign
		q = wire_mgl_prim1_q;
endmodule //mg9ob
//VALID FILE
