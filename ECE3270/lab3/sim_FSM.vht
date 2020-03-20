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
-- Generated on "10/31/2019 20:20:20"
                                                            
-- Vhdl Test Bench template for design  :  sim_FSM
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY sim_FSM_vhd_tst IS
END sim_FSM_vhd_tst;
ARCHITECTURE sim_FSM_arch OF sim_FSM_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL AddSubf : STD_LOGIC;
SIGNAL Ainf : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL DINoutf : STD_LOGIC;
SIGNAL Done : STD_LOGIC;
SIGNAL Ginf : STD_LOGIC;
SIGNAL Goutf : STD_LOGIC;
SIGNAL IRinf : STD_LOGIC;
SIGNAL IRoutf : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL Resetn : STD_LOGIC;
SIGNAL Rinf : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL Routf : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL Run : STD_LOGIC;
COMPONENT sim_FSM
	PORT (
	AddSubf : OUT STD_LOGIC;
	Ainf : OUT STD_LOGIC;
	clk : IN STD_LOGIC;
	DINoutf : OUT STD_LOGIC;
	Done : OUT STD_LOGIC;
	Ginf : OUT STD_LOGIC;
	Goutf : OUT STD_LOGIC;
	IRinf : OUT STD_LOGIC;
	IRoutf : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	Resetn : IN STD_LOGIC;
	Rinf : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	Routf : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	Run : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : sim_FSM
	PORT MAP (
-- list connections between master ports and signals
	AddSubf => AddSubf,
	Ainf => Ainf,
	clk => clk,
	DINoutf => DINoutf,
	Done => Done,
	Ginf => Ginf,
	Goutf => Goutf,
	IRinf => IRinf,
	IRoutf => IRoutf,
	Resetn => Resetn,
	Rinf => Rinf,
	Routf => Routf,
	Run => Run
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
-- (       )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
   Resetn <= '0';
	Run <= '0';
	IRoutf <= "00000000";-- At first, we clear the IR
	wait for 10ns;
	
	Run <= '1';
	Resetn <= '1';
	IRoutf <= "00111101";-- Second, we set the IRoutf for  copy the R5 to R7
	wait for 10ns;
	Run <= '0';
	wait for 10ns;
	
	Run <= '1';
	IRoutf <= "01110101";-- Third, we set the IRoutf for copy the DIN to R6
	wait for 10ns;
	Run <= '0';
	wait for 10ns;
	Run <= '1';
	IRoutf <= "10111101";-- Fourth, we set the R5 + R7
	wait for 10ns;
	run <= '0';
	wait for 30ns;
	
	run <= '1';
	IRoutf <= "11111101";-- Fifth, we set teh R5 - R7
	wait for 10ns;
	run <= '0';	
   wait for 30ns;
	
	std.env.stop(0);
WAIT;                                                        
END PROCESS always;                                          
END sim_FSM_arch;
