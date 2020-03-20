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
-- Generated on "09/26/2019 15:03:09"
                                                            
-- Vhdl Test Bench template for design  :  lab1p1top4
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY lab1p1top4_vhd_tst IS
END lab1p1top4_vhd_tst;
ARCHITECTURE lab1p1top4_arch OF lab1p1top4_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL input4 : STD_LOGIC_VECTOR(3 DOWNTO 0);
SIGNAL output7 : STD_LOGIC_VECTOR(6 DOWNTO 0);
SIGNAL select_next : STD_LOGIC;
SIGNAL valid_fib : STD_LOGIC;
COMPONENT lab1p1top4
	PORT (
	input4 : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
	output7 : OUT STD_LOGIC_VECTOR(6 DOWNTO 0);
	select_next : IN STD_LOGIC;
	valid_fib : OUT STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : lab1p1top4
	PORT MAP (
-- list connections between master ports and signals
	input4 => input4,
	output7 => output7,
	select_next => select_next,
	valid_fib => valid_fib
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
		  input4 <= "0000";
		  select_next <= '0';
		  wait for 10ns;
		  input4 <= "0001";
		  select_next <= '0';
		  wait for 10ns;
		  input4 <= "0100";
		  select_next <= '0';
		  wait for 10ns;
		  input4 <= "1101";
		  select_next <= '0';
		  wait for 10ns;
		  input4 <= "0000";
		  select_next <= '1';
		  wait for 10ns;
		  input4 <= "0001";
		  select_next <= '1';
		  wait for 10ns;
		  input4 <= "0100";
		  select_next <= '1';
		  wait for 10ns;
		  input4 <= "1101";
		  select_next <= '1';
		  wait for 10ns;
		  
WAIT;                                                        
END PROCESS always;                                          
END lab1p1top4_arch;
