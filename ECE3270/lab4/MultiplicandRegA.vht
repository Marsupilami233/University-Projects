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
-- Generated on "11/08/2019 15:22:58"
                                                            
-- Vhdl Test Bench template for design  :  MultiplicandRegA
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY MultiplicandRegA_vhd_tst IS
END MultiplicandRegA_vhd_tst;
ARCHITECTURE MultiplicandRegA_arch OF MultiplicandRegA_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL D : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL loadreg : STD_LOGIC;
SIGNAL Q1 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q2 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q_1 : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL Q_2 : STD_LOGIC_VECTOR(8 DOWNTO 0);
COMPONENT MultiplicandRegA
	PORT (
	clk : IN STD_LOGIC;
	D : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	loadreg : IN STD_LOGIC;
	Q1 : BUFFER STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q2 : BUFFER STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q_1 : BUFFER STD_LOGIC_VECTOR(8 DOWNTO 0);
	Q_2 : BUFFER STD_LOGIC_VECTOR(8 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : MultiplicandRegA
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	D => D,
	loadreg => loadreg,
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
PROCESS BEGIN
clk <= '0';
wait for 5ns;
clk <= '1';
wait for 5ns;
END PROCESS;                                          
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  D <= "00000000";
		  loadreg <= '0';
		  wait for 10ns;
		  D <= "10101010";
		  loadreg <= '1';
		  wait for 10ns;
		  D <= "01011111";
		  wait for 10ns;
		  loadreg <= '0';
		  wait for 10ns;
		  std.env.stop(0);
		  
WAIT;                                                        
END PROCESS always;                                          
END MultiplicandRegA_arch;
