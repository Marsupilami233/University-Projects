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
-- Generated on "11/15/2019 10:21:08"
                                                            
-- Vhdl Test Bench template for design  :  ShiftRegD
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY ShiftRegD_vhd_tst IS
END ShiftRegD_vhd_tst;
ARCHITECTURE ShiftRegD_arch OF ShiftRegD_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL count : STD_LOGIC;
SIGNAL loadreg : STD_LOGIC;
SIGNAL Q : STD_LOGIC_VECTOR(2 DOWNTO 0);
COMPONENT ShiftRegD
	PORT (
	clk : IN STD_LOGIC;
	count : IN STD_LOGIC;
	loadreg : IN STD_LOGIC;
	Q : OUT STD_LOGIC_VECTOR(2 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : ShiftRegD
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	count => count,
	loadreg => loadreg,
	Q => Q
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once                      
WAIT;                                                       
END PROCESS init;
PROCESS BEGIN
clk <='0';
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
		          -- code executes for every event on sensitivity list  
		  loadreg <= '0';
		  count <= '0';
		  wait for 10ns; -- test for the wanted count value
		  loadreg <= '1';
		  count <= '0';
		  wait for 10ns; -- test for the wanted  input value
		  loadreg <= '0'; 
        count <= '1'; -- the output should respond to the count value
        wait for 40ns;
        
		  loadreg <= '0';
		  count <= '0';
		  wait for 10ns; -- test for the wanted count value
		  loadreg <= '1';
		  count <= '0';
		  wait for 10ns; -- test for the wanted  input value
		  loadreg <= '0'; 
        count <= '1'; -- the output should respond to the count value
        wait for 40ns;		  
		  std.env.stop(0);
		  
WAIT;                                                        
END PROCESS always;                                          
END ShiftRegD_arch;
