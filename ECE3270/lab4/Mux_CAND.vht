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
-- Generated on "11/12/2019 14:33:12"
                                                            
-- Vhdl Test Bench template for design  :  Mux_CAND
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Mux_CAND_vhd_tst IS
END Mux_CAND_vhd_tst;
ARCHITECTURE Mux_CAND_arch OF Mux_CAND_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL BIT_WISE : STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL OUTPUT : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q0 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q1 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q2 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q_1 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q_2 : STD_LOGIC_VECTOR(8 DOWNTO 0);
COMPONENT Mux_CAND
	PORT (
	BIT_WISE : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
	OUTPUT : OUT STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q0 : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q1 : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q2 : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q_1 : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q_2 : IN STD_LOGIC_VECTOR(8 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : Mux_CAND
	PORT MAP (
-- list connections between master ports and signals
	BIT_WISE => BIT_WISE,
	OUTPUT => OUTPUT,
	Q0 => Q0,
	Q1 => Q1,
	Q2 => Q2,
	Q_1 => Q_1,
	Q_2 => Q_2
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
		  
		  BIT_WISE <= "000";
		  Q1  <= "001011111";
		  Q2  <= "010111110";
		  Q_2 <= "101000010";
		  Q_1 <= "110100001";
		  Q0  <= "000000000";
		  wait for 5ns;
		  BIT_WISE <= "010";
		  wait for 5ns;
		  BIT_WISE <= "011";
		  wait for 5ns;
		  BIT_WISE <= "100";
		  wait for 5ns;
WAIT;                                                        
END PROCESS always;                                          
END Mux_CAND_arch;
