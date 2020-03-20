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
-- Generated on "10/29/2019 17:34:22"
                                                            
-- Vhdl Test Bench template for design  :  Registers
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Registers_vhd_tst IS
END Registers_vhd_tst;
ARCHITECTURE Registers_arch OF Registers_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL D : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL Input_select : STD_LOGIC;
SIGNAL Output : STD_LOGIC_VECTOR(15 DOWNTO 0);
COMPONENT Registers
	PORT (
	clk : IN STD_LOGIC;
	D : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	Input_select : IN STD_LOGIC;
	Output : OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : Registers
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	D => D,
	Input_select => Input_select,
	Output => Output
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once 	  
WAIT;                                                       
END PROCESS init;
PROCESS
BEGIN
   clk <= '0';
	wait for 5ns;
	clk <= '1';
	wait for 5ns;
END PROCESS;                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- ()                                                 
-- variable declarations                                   
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  D <= x"0010";
		  Input_select <= '0';	
        wait for 5ns;
		  Input_select <= '1';
		  wait for 10ns;
		  Input_select <= '0';
		  D <= x"0100";
		  wait for 10ns;
		  Input_select <= '1';
		  wait for 10ns;
		  --- Discusson the result of the Output should be first set on to unknown state
		  --- Then after the first clock cycle, the output should be first set to hex 0010,
		  --- Next the output should stay on what the last output
		  --- Finally, on the last rising-edge the signal will update to another value
		  
WAIT;                                                        
END PROCESS always;                                          
END Registers_arch;
