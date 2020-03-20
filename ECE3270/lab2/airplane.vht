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
-- Generated on "10/17/2019 21:09:24"
                                                            
-- Vhdl Test Bench template for design  :  airplane
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY airplane_vhd_tst IS
END airplane_vhd_tst;
ARCHITECTURE airplane_arch OF airplane_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL alt_10k : STD_LOGIC;
SIGNAL alt_25k : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL ivalid : STD_LOGIC;
SIGNAL noelectronics : STD_LOGIC;
SIGNAL reset : STD_LOGIC;
SIGNAL seatbelt : STD_LOGIC;
SIGNAL smooth : STD_LOGIC;
COMPONENT airplane
	PORT (
	alt_10k : IN STD_LOGIC;
	alt_25k : IN STD_LOGIC;
	clk : IN STD_LOGIC;
	ivalid : IN STD_LOGIC;
	noelectronics : OUT STD_LOGIC;
	reset : IN STD_LOGIC;
	seatbelt : OUT STD_LOGIC;
	smooth : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : airplane
	PORT MAP (
-- list connections between master ports and signals
	alt_10k => alt_10k,
	alt_25k => alt_25k,
	clk => clk,
	ivalid => ivalid,
	noelectronics => noelectronics,
	reset => reset,
	seatbelt => seatbelt,
	smooth => smooth
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
	--alt_10k <= '0';
	--alt_25k <= '0';
	--clk <= '0';
	--ivalid <= '0';
	--noelectronics <= '1';
	--reset <= '1';
	--seatbelt <= '1';
	--smooth <= '0';                       
WAIT;                                                       
END PROCESS init;
clk_process :PROCESS
BEGIN
   clk <= '0';
   wait for 5ns;  -- for 5ns
   clk <= '1';
   wait for 5ns;  -- for next 5ns the signal will be '1'
END PROCESS clk_process;
                                       
always : PROCESS                                                                                 
BEGIN 
   reset <= '0'; 
	wait for 10ns; reset <= '1';  --reset for only one clock cycle
	ivalid <= '1';
   wait for 10ns; smooth <= '1';
   wait for 10ns; smooth <= '1';
   wait for 10ns; smooth <= '1';
   wait for 10ns; smooth <= '1';
	wait for 10ns; smooth <= '1'; -- test that the seat belt went well in specific situations, and will bot go down

	wait for 10ns; alt_10k <= '1';
   wait for 10ns; alt_10k <= '1'; -- noelectronic will toggle a little bit when the plane go up and down at this point
	
	wait for 10ns; alt_10k <= '0';  -- It seems impossible if the alt_10k as not be low and the alt_25k goes high,but if that happens
	wait for 10ns; alt_10k <= '1';
	
   wait for 10ns; alt_10k <= '0';  -- assume the plane will go beyond the 25k feet
	wait for 10ns; alt_25k <= '1';  -- noelectronic will be low in this clock cycle and the plane will go from below 10k to 10k-25k
	                         
	wait for 10ns; alt_25k <= '0';
	wait for 10ns; alt_25k <= '1';  -- the planes altitude will first go down to 10k-25 range and then go up to 25k above
	wait for 10ns; alt_25k <= '1';
	
	wait for 10ns; alt_25k <= '0';
	
	wait for 10ns; smooth <= '0';
	wait for 10ns; smooth <= '1';  -- smooth for only one clock cycle
	
	wait for 20ns; smooth <= '0';
	wait for 10ns; smooth <= '1';  -- smooth for two clock cycle
	
	wait for 30ns; smooth <= '0';
	wait for 10ns; smooth <= '1';  -- smooth for three cycle

	wait for 40ns; smooth <= '0';
	wait for 10ns; smooth <= '1';  -- smooth for four cycle

	wait for 50ns; smooth <= '0'; -- the seat belt should be low in this clock cycle
	wait for 10ns; smooth <= '1'; -- smooth for five cycle
   
	wait for 10ns; alt_25k <= '1';
	wait for 10ns; alt_25k <= '0';-- the plane should return to its state of the below_25k and above_10k
	wait for 10ns; alt_10k <= '1';
	wait for 10ns; alt_10k <= '0';-- the plane should return to its original state
	
	
WAIT;                                                        
END PROCESS always;                                          
END airplane_arch;

