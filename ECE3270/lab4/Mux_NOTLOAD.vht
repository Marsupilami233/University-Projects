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
-- Generated on "11/15/2019 17:28:56"
                                                            
-- Vhdl Test Bench template for design  :  Mux_NOTLOAD
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Mux_NOTLOAD_vhd_tst IS
END Mux_NOTLOAD_vhd_tst;
ARCHITECTURE Mux_NOTLOAD_arch OF Mux_NOTLOAD_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL D : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL NOTloadreg : STD_LOGIC;
SIGNAL Q : STD_LOGIC_VECTOR(8 DOWNTO 0);
--SIGNAL Q0 : STD_LOGIC_VECTOR(8 DOWNTO 0);
COMPONENT Mux_NOTLOAD
	PORT (
	D : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	NOTloadreg : IN STD_LOGIC;
	Q : OUT STD_LOGIC_VECTOR(8 DOWNTO 0)
	--Q0 : IN STD_LOGIC_VECTOR(8 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : Mux_NOTLOAD
	PORT MAP (
-- list connections between master ports and signals
	D => D,
	NOTloadreg => NOTloadreg,
	Q => Q
	--Q0 => Q0
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
		  D  <= "000011111";
		  --Q0 <= "000000000";
		  NOTloadreg <= '1';
		  wait for 5ns;
		  NOTloadreg <= '0';
		  wait for 5ns;
		  D  <= "111100000";
        wait for 5ns;		  
WAIT;                                                        
END PROCESS always;                                          
END Mux_NOTLOAD_arch;