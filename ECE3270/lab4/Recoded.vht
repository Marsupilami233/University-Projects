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
-- Generated on "11/15/2019 17:49:52"
                                                            
-- Vhdl Test Bench template for design  :  Recoded
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Recoded_vhd_tst IS
END Recoded_vhd_tst;
ARCHITECTURE Recoded_arch OF Recoded_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL Busy : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL Done : STD_LOGIC;
SIGNAL Multiplicand : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL Multiplier : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL ProductH : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL ProductL : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL Resetn : STD_LOGIC;
SIGNAL Start : STD_LOGIC;
COMPONENT Recoded
	PORT (
	Busy : BUFFER STD_LOGIC;
	clk : IN STD_LOGIC;
	Done : BUFFER STD_LOGIC;
	Multiplicand : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	Multiplier : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	ProductH : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0);
	ProductL : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0);
	Resetn : IN STD_LOGIC;
	Start : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : Recoded
	PORT MAP (
-- list connections between master ports and signals
	Busy => Busy,
	clk => clk,
	Done => Done,
	Multiplicand => Multiplicand,
	Multiplier => Multiplier,
	ProductH => ProductH,
	ProductL => ProductL,
	Resetn => Resetn,
	Start => Start
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
END PROCESS ;                                                
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  Resetn <= '0';
		  Multiplicand <= "00000010";
		  Multiplier   <= "00000010";
		  start        <= '1';
		  wait for 10ns;
		  Resetn       <= '1';
		  start        <= '1';
		  wait for 120ns;
		  std.env.stop(0);
WAIT;                                                        
END PROCESS always;                                          
END Recoded_arch;
