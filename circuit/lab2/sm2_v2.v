// megafunction wizard: %LPM_ADD_SUB%
// GENERATION: STANDARD
// VERSION: WM1.0
// MODULE: LPM_ADD_SUB 

// ============================================================
// File Name: sm2_v2.v
// Megafunction Name(s):
// 			LPM_ADD_SUB
//
// Simulation Library Files(s):
// 			lpm
// ============================================================
// ************************************************************
// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
//
// 19.1.0 Build 670 09/22/2019 SJ Lite Edition
// ************************************************************


//Copyright (C) 2019  Intel Corporation. All rights reserved.
//Your use of Intel Corporation's design tools, logic functions 
//and other software and tools, and any partner logic 
//functions, and any output files from any of the foregoing 
//(including device programming or simulation files), and any 
//associated documentation or information are expressly subject 
//to the terms and conditions of the Intel Program License 
//Subscription Agreement, the Intel Quartus Prime License Agreement,
//the Intel FPGA IP License Agreement, or other applicable license
//agreement, including, without limitation, that your use is for
//the sole purpose of programming logic devices manufactured by
//Intel and sold by Intel or its authorized distributors.  Please
//refer to the applicable agreement for further details, at
//https://fpgasoftware.intel.com/eula.


// synopsys translate_off
`timescale 1 ps / 1 ps
// synopsys translate_on
module sm2_v2 (
	cin,
	dataa,
	datab,
	cout,
	result);

	input	  cin;
	input	[0:0]  dataa;
	input	[0:0]  datab;
	output	  cout;
	output	[0:0]  result;

	wire  sub_wire0;
	wire [0:0] sub_wire1;
	wire  cout = sub_wire0;
	wire [0:0] result = sub_wire1[0:0];

	lpm_add_sub	LPM_ADD_SUB_component (
				.cin (cin),
				.dataa (dataa),
				.datab (datab),
				.cout (sub_wire0),
				.result (sub_wire1)
				// synopsys translate_off
				,
				.aclr (),
				.add_sub (),
				.clken (),
				.clock (),
				.overflow ()
				// synopsys translate_on
				);
	defparam
		LPM_ADD_SUB_component.lpm_direction = "ADD",
		LPM_ADD_SUB_component.lpm_hint = "ONE_INPUT_IS_CONSTANT=NO,CIN_USED=YES",
		LPM_ADD_SUB_component.lpm_representation = "UNSIGNED",
		LPM_ADD_SUB_component.lpm_type = "LPM_ADD_SUB",
		LPM_ADD_SUB_component.lpm_width = 1;


endmodule

// ============================================================
// CNX file retrieval info
// ============================================================
// Retrieval info: PRIVATE: CarryIn NUMERIC "1"
// Retrieval info: PRIVATE: CarryOut NUMERIC "1"
// Retrieval info: PRIVATE: ConstantA NUMERIC "0"
// Retrieval info: PRIVATE: ConstantB NUMERIC "0"
// Retrieval info: PRIVATE: Function NUMERIC "0"
// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "Cyclone V"
// Retrieval info: PRIVATE: LPM_PIPELINE NUMERIC "0"
// Retrieval info: PRIVATE: Latency NUMERIC "0"
// Retrieval info: PRIVATE: Overflow NUMERIC "0"
// Retrieval info: PRIVATE: RadixA NUMERIC "10"
// Retrieval info: PRIVATE: RadixB NUMERIC "10"
// Retrieval info: PRIVATE: Representation NUMERIC "1"
// Retrieval info: PRIVATE: SYNTH_WRAPPER_GEN_POSTFIX STRING "0"
// Retrieval info: PRIVATE: ValidCtA NUMERIC "0"
// Retrieval info: PRIVATE: ValidCtB NUMERIC "0"
// Retrieval info: PRIVATE: WhichConstant NUMERIC "0"
// Retrieval info: PRIVATE: aclr NUMERIC "0"
// Retrieval info: PRIVATE: clken NUMERIC "0"
// Retrieval info: PRIVATE: nBit NUMERIC "1"
// Retrieval info: PRIVATE: new_diagram STRING "1"
// Retrieval info: LIBRARY: lpm lpm.lpm_components.all
// Retrieval info: CONSTANT: LPM_DIRECTION STRING "ADD"
// Retrieval info: CONSTANT: LPM_HINT STRING "ONE_INPUT_IS_CONSTANT=NO,CIN_USED=YES"
// Retrieval info: CONSTANT: LPM_REPRESENTATION STRING "UNSIGNED"
// Retrieval info: CONSTANT: LPM_TYPE STRING "LPM_ADD_SUB"
// Retrieval info: CONSTANT: LPM_WIDTH NUMERIC "1"
// Retrieval info: USED_PORT: cin 0 0 0 0 INPUT NODEFVAL "cin"
// Retrieval info: USED_PORT: cout 0 0 0 0 OUTPUT NODEFVAL "cout"
// Retrieval info: USED_PORT: dataa 0 0 1 0 INPUT NODEFVAL "dataa[0..0]"
// Retrieval info: USED_PORT: datab 0 0 1 0 INPUT NODEFVAL "datab[0..0]"
// Retrieval info: USED_PORT: result 0 0 1 0 OUTPUT NODEFVAL "result[0..0]"
// Retrieval info: CONNECT: @cin 0 0 0 0 cin 0 0 0 0
// Retrieval info: CONNECT: @dataa 0 0 1 0 dataa 0 0 1 0
// Retrieval info: CONNECT: @datab 0 0 1 0 datab 0 0 1 0
// Retrieval info: CONNECT: cout 0 0 0 0 @cout 0 0 0 0
// Retrieval info: CONNECT: result 0 0 1 0 @result 0 0 1 0
// Retrieval info: GEN_FILE: TYPE_NORMAL sm2_v2.v TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL sm2_v2.inc FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL sm2_v2.cmp FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL sm2_v2.bsf TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL sm2_v2_inst.v TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL sm2_v2_bb.v FALSE
// Retrieval info: LIB_FILE: lpm
