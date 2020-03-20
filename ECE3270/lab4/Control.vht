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
-- Generated on "11/15/2019 11:07:50"
                                                            
-- Vhdl Test Bench template for design  :  Control
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Control_vhd_tst IS
END Control_vhd_tst;
ARCHITECTURE Control_arch OF Control_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL Addreg : STD_LOGIC;
--SIGNAL BitPair : STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL Busy : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL Count : STD_LOGIC;
SIGNAL Done : STD_LOGIC;
SIGNAL Loadreg : STD_LOGIC;
SIGNAL RegD : STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL Resetn : STD_LOGIC;
--SIGNAL Shift : STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL Shiftreg : STD_LOGIC;
SIGNAL Start : STD_LOGIC;
COMPONENT Control
	PORT (
	Addreg : OUT STD_LOGIC;
--	BitPair : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
	Busy : OUT STD_LOGIC;
	clk : IN STD_LOGIC;
	Count : OUT STD_LOGIC;
	Done : OUT STD_LOGIC;
	Loadreg : OUT STD_LOGIC;
	RegD : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
	Resetn : IN STD_LOGIC;
--	Shift : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
	Shiftreg : OUT STD_LOGIC;
	Start : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : Control
	PORT MAP (
-- list connections between master ports and signals
	Addreg => Addreg,
--	BitPair => BitPair,
	Busy => Busy,
	clk => clk,
	Count => Count,
	Done => Done,
	Loadreg => Loadreg,
	RegD => RegD,
	Resetn => Resetn,
--	Shift => Shift,
	Shiftreg => Shiftreg,
	Start => Start
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
		     Resetn <= '0';
	Start <= '0';
--	Shift <= "000";-- At first, we clear the IR
	RegD  <= "000"; 
	wait for 10ns;
	
	Start <= '1';
	Resetn <= '1';
	RegD <= "100";-- Second, we set the IRoutf for  copy the R5 to R7
	wait for 10ns;
	Start <= '0';
   wait for 10ns;
	
	RegD <= "011";-- Third, we set the IRoutf for  copy the R5 to R7
	wait for 10ns;
	wait for 10ns;
	

	RegD <= "010";-- Fourth, we set the IRoutf for  copy the R5 to R7
	wait for 10ns;
	wait for 10ns;	
	
	RegD <= "001";-- Fourth, we set the IRoutf for  copy the R5 to R7
	wait for 10ns;
	wait for 10ns;

	RegD <= "000";-- Fourth, we set the IRoutf for  copy the R5 to R7
	wait for 30ns;
	std.env.stop(0);
		  
WAIT;                                                        
END PROCESS always;                                          
END Control_arch;
