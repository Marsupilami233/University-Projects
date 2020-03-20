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
-- Generated on "11/17/2019 01:50:35"
                                                            
-- Vhdl Test Bench template for design  :  Adder
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Adder_vhd_tst IS
END Adder_vhd_tst;
ARCHITECTURE Adder_arch OF Adder_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL AddResult : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Carry : STD_LOGIC;
SIGNAL Datain : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL RegCin : STD_LOGIC_VECTOR(8 DOWNTO 0);
COMPONENT Adder
	PORT (
	AddResult : OUT STD_LOGIC_VECTOR(8 DOWNTO 0);
	Carry : OUT STD_LOGIC;
	Datain : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	RegCin : IN STD_LOGIC_VECTOR(8 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : Adder
	PORT MAP (
-- list connections between master ports and signals
	AddResult => AddResult,
	Carry => Carry,
	Datain => Datain,
	RegCin => RegCin
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
		  RegCin <= "111111111";
		  Datain <= "000000011";
		  wait for 5ns; 
WAIT;                                                          
END PROCESS always;                                          
END Adder_arch;
