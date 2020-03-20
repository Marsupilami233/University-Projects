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
-- Generated on "11/17/2019 11:13:00"
                                                            
-- Vhdl Test Bench template for design  :  RegC
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY RegC_vhd_tst IS
END RegC_vhd_tst;
ARCHITECTURE RegC_arch OF RegC_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL addreg : STD_LOGIC;
SIGNAL Carry : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL D : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL loadreg : STD_LOGIC;
SIGNAL Q : STD_LOGIC_VECTOR(8 DOWNTO 0);
SIGNAL shift_out : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL shiftreg : STD_LOGIC;
COMPONENT RegC
	PORT (
	addreg : IN STD_LOGIC;
	Carry : IN STD_LOGIC;
	clk : IN STD_LOGIC;
	D : IN STD_LOGIC_VECTOR(8 DOWNTO 0);
	loadreg : IN STD_LOGIC;
	Q : BUFFER STD_LOGIC_VECTOR(8 DOWNTO 0);
	shift_out : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	shiftreg : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : RegC
	PORT MAP (
-- list connections between master ports and signals
	addreg => addreg,
	Carry => Carry,
	clk => clk,
	D => D,
	loadreg => loadreg,
	Q => Q,
	shift_out => shift_out,
	shiftreg => shiftreg
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
		  Carry <= '1';
		  D <= "010010000";
		  loadreg <= '1';
		  addreg <= '0';
		  wait for 10ns;
		  loadreg <= '0';
--		  addreg  <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg  <= '0';
		  wait for 10ns;
		  
		  shiftreg <= '0';
--		  addreg  <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
		  addreg  <= '0';
		  wait for 10ns;
		  
		  shiftreg <= '0';
--		  addreg <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg <= '0';
		  wait for 10ns;	
		  
		  shiftreg <= '0';
--		  addreg <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg <= '0';
		  wait for 10ns;	

		  shiftreg <= '0';
--		  addreg <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg <= '0';
		  wait for 10ns;	
        
        D <= "101001101";
--        addreg <= '1';
        loadreg <= '1';
        shiftreg <= '0';
        wait for 10ns;
		  loadreg <= '0';
--		  addreg <= '0';
        shiftreg <= '1';
        wait for 10ns;

		  shiftreg <= '0';
--		  addreg <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg <= '0';
		  wait for 10ns;	

		  shiftreg <= '0';
--		  addreg <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg <= '0';
		  wait for 10ns;	

		  shiftreg <= '0';
--		  addreg <= '1';
		  wait for 10ns;
		  shiftreg <= '1';
--		  addreg <= '0';
		  wait for 10ns;	
        wait for 20ns;		  
		  std.env.stop(0);  		  
WAIT;                                                        
END PROCESS always;                                          
END RegC_arch;
