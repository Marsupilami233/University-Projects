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
-- Generated on "09/25/2019 15:07:22"
                                                            
-- Vhdl Test Bench template for design  :  seven_seg_decoder
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY seven_seg_decoder_vhd_tst IS
END seven_seg_decoder_vhd_tst;
ARCHITECTURE seven_seg_decoder_arch OF seven_seg_decoder_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL f : STD_LOGIC_VECTOR(6 DOWNTO 0);
SIGNAL x : STD_LOGIC_VECTOR(3 DOWNTO 0);
COMPONENT seven_seg_decoder
	PORT (
	f : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
	x : IN STD_LOGIC_VECTOR(3 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : seven_seg_decoder
	PORT MAP (
-- list connections between master ports and signals
	f => f,
	x => x
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
		  x <= "0000";
		  wait for 10ns;
		  x <= "0001";
		  wait for 10ns;
		  x <= "0010";
		  wait for 10ns;
		  x <= "0011";
		  wait for 10ns;
		  x <= "0101";
		  wait for 10ns;
		  x <= "1000";
		  wait for 10ns;
		  x <= "1001";
		  wait for 10ns;
		  x <= "1111";
		  wait for 10ns;
		  x <= "0101";
		  wait for 10ns;
WAIT;                                                        
END PROCESS always;                                          
END seven_seg_decoder_arch;
