-- Copyright (C) 1991-2016 Altera Corporation. All rights reserved.
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, the Altera Quartus Prime License Agreement,
-- the Altera MegaCore Function License Agreement, or other 
-- applicable license agreement, including, without limitation, 
-- that your use is for the sole purpose of programming logic 
-- devices manufactured by Altera and sold by Altera or its 
-- authorized distributors.  Please refer to the applicable 
-- agreement for further details.

-- ***************************************************************************
-- This file contains a Vhdl test bench template that is freely editable to   
-- suit user's needs .Comments are provided in each section to help the user  
-- fill out necessary details.                                                
-- ***************************************************************************
-- Generated on "10/31/2019 15:46:04"
                                                            
-- Vhdl Test Bench template for design  :  mux_vary
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY mux_vary_vhd_tst IS
END mux_vary_vhd_tst;
ARCHITECTURE mux_vary_arch OF mux_vary_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL Bus_out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL DINout : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL DINout_select : STD_LOGIC;
SIGNAL Gout : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL Gout_select : STD_LOGIC;
SIGNAL R0out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R0out_select : STD_LOGIC;
SIGNAL R1out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R1out_select : STD_LOGIC;
SIGNAL R2out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R2out_select : STD_LOGIC;
SIGNAL R3out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R3out_select : STD_LOGIC;
SIGNAL R4out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R4out_select : STD_LOGIC;
SIGNAL R5out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R5out_select : STD_LOGIC;
SIGNAL R6out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R6out_select : STD_LOGIC;
SIGNAL R7out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL R7out_select : STD_LOGIC;
COMPONENT mux_vary
	PORT (
	Bus_out : BUFFER STD_LOGIC_VECTOR(15 DOWNTO 0);
	DINout : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	DINout_select : IN STD_LOGIC;
	Gout : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	Gout_select : IN STD_LOGIC;
	R0out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R0out_select : IN STD_LOGIC;
	R1out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R1out_select : IN STD_LOGIC;
	R2out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R2out_select : IN STD_LOGIC;
	R3out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R3out_select : IN STD_LOGIC;
	R4out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R4out_select : IN STD_LOGIC;
	R5out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R5out_select : IN STD_LOGIC;
	R6out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R6out_select : IN STD_LOGIC;
	R7out : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	R7out_select : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : mux_vary
	PORT MAP (
-- list connections between master ports and signals
	Bus_out => Bus_out,
	DINout => DINout,
	DINout_select => DINout_select,
	Gout => Gout,
	Gout_select => Gout_select,
	R0out => R0out,
	R0out_select => R0out_select,
	R1out => R1out,
	R1out_select => R1out_select,
	R2out => R2out,
	R2out_select => R2out_select,
	R3out => R3out,
	R3out_select => R3out_select,
	R4out => R4out,
	R4out_select => R4out_select,
	R5out => R5out,
	R5out_select => R5out_select,
	R6out => R6out,
	R6out_select => R6out_select,
	R7out => R7out,
	R7out_select => R7out_select
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once                      
WAIT;                                                       
END PROCESS init;                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
WAIT;                                                        
END PROCESS always;                                          
END mux_vary_arch;
