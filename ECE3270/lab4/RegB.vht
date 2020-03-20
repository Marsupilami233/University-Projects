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
-- Generated on "11/15/2019 16:17:13"
                                                            
-- Vhdl Test Bench template for design  :  RegB
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY RegB_vhd_tst IS
END RegB_vhd_tst;
ARCHITECTURE RegB_arch OF RegB_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL D : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL loadreg : STD_LOGIC;
SIGNAL Q : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL shiftin : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL bitpair : STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL shiftreg : STD_LOGIC;
COMPONENT RegB
	PORT (
	clk : IN STD_LOGIC;
	D : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	loadreg : IN STD_LOGIC;
	Q : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	shiftin : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
	bitpair : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
	shiftreg : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : RegB
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	D => D,
	loadreg => loadreg,
	Q => Q,
	shiftin => shiftin,
	bitpair => bitpair,
	shiftreg => shiftreg
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
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
        D <= "10100110";
		  loadreg <= '1';
        shiftin <= "00";
		  wait for 10ns;
		  loadreg <= '0';
		  shiftreg <= '1';
		  wait for 10ns;
		  
  		  shiftin <= "01";
		  shiftreg <= '0';
		  wait for 10ns;
		  shiftreg <= '1';
		  wait for 10ns;
		  
  		  shiftin <= "10";		  
		  shiftreg <= '0';
		  wait for 10ns;
		  shiftreg <= '1';
		  wait for 10ns;	

 		  shiftin <= "11";			  
		  shiftreg <= '0';
		  wait for 10ns;
		  shiftreg <= '1';
		  wait for 10ns;
        
        loadreg <= '1';
		  D <= "01001000";
        shiftreg <= '0';
		  
        shiftin <= "00";
		  wait for 10ns;
		  loadreg <= '0';
		  shiftreg <= '1';
		  wait for 10ns;
		  
		  shiftin <= "01";
		  shiftreg <= '0';
		  wait for 10ns;
		  shiftreg <= '1';
		  wait for 10ns;
		  
		  shiftin <= "10";		  
		  shiftreg <= '0';
		  wait for 10ns;
		  shiftreg <= '1';
		  wait for 10ns;	

		  shiftin <= "11";			  
		  shiftreg <= '0';
		  wait for 10ns;
		  shiftreg <= '1';
		  wait for 10ns;	
		  shiftreg <= '0';
        wait for 20ns;		  
		  std.env.stop(0);  		  
WAIT;                                                        
END PROCESS always;                                          
END RegB_arch;