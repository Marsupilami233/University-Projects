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
-- Generated on "10/29/2019 15:48:02"
                                                            
-- Vhdl Test Bench template for design  :  AddSub
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY AddSub_vhd_tst IS
END AddSub_vhd_tst;
ARCHITECTURE AddSub_arch OF AddSub_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL AddOrSub : STD_LOGIC;
SIGNAL AddSubout : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL AoutinAd : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL BusinAd : STD_LOGIC_VECTOR(15 DOWNTO 0);
COMPONENT AddSub
	PORT (
	AddOrSub : IN STD_LOGIC;
	AddSubout : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	AoutinAd : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	BusinAd : IN STD_LOGIC_VECTOR(15 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : AddSub
	PORT MAP (
-- list connections between master ports and signals
	AddOrSub => AddOrSub,
	AddSubout => AddSubout,
	AoutinAd => AoutinAd,
	BusinAd => BusinAd
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once                      
WAIT;                                                       
END PROCESS init;                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- (    )                                                  
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		 AddOrSub <= '0'; 
		 AoutinAd <= x"0008"; 
		 BusinAd <= x"0003";
		 wait for 5ns; -- initial state
		 AddOrSub <= '1';
		 wait for 5ns;
		 AddOrSub <= '0';
		 wait for 5ns;
		 AoutinAd <= x"0000";
		 BusinAd <=  x"0004";
		 wait for 5ns;
		 AddOrSub <= '1';
		 wait for 5ns;
		  
WAIT;                                                        
END PROCESS always;                                          
END AddSub_arch;
