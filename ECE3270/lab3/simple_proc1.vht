-- copyright (C) 1991-2016 Altera Corporation. All rights reserved.
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
-- Generated on "11/01/2019 20:40:24"
                                                            
-- Vhdl Test Bench template for design  :  simple_proc
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY simple_proc_vhd_tst1 IS
END simple_proc_vhd_tst1;
ARCHITECTURE simple_proc_arch OF simple_proc_vhd_tst1 IS
-- constants                                                 
-- signals                                                   
SIGNAL bus_proc : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL clk : STD_LOGIC;
SIGNAL DIN : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL done : STD_LOGIC;
SIGNAL rstn : STD_LOGIC;
SIGNAL run : STD_LOGIC;
COMPONENT simple_proc
	PORT (
	bus_proc : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	clk : IN STD_LOGIC;
	DIN : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	done : OUT STD_LOGIC;
	rstn : IN STD_LOGIC;
	run : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : simple_proc
	PORT MAP (
-- list connections between master ports and signals
	bus_proc => bus_proc,
	clk => clk,
	DIN => DIN,
	done => done,
	rstn => rstn,
	run => run
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
		          -- code executes for every event on sensitivity list  
   rstn <= '0';
	run <= '0';
	DIN <= "1111000001000000";  --mv 0,DIN 11110000_01000000  R0 : 11110000
	wait for 10ns;
	run <= '1';
	rstn <= '1';
	wait for 10ns;
	run <= '0';
	-- set run to 1 in one clock cycle
	
        wait for 10ns;
		  run <= '1';
        DIN <= "0000111101001000"; --mv 1,DIN	00001111_01001000 R1 :00001111
		  wait for 10ns;
		  run <= '0';
        wait for 10ns;
		  run <= '1';
        DIN <= "0000000010000001";  --add 0,1  11111111  00000000_10000001 R1: 11111111
		  wait for 10ns;
		  run <= '0';
	     wait for 40ns;	

	
		  run <= '1';
	     DIN <= "0000000011000001";  --sub 0,1 11111111 - 00001111 = 11110000
		  wait for 10ns;
		  run <= '0';
	     wait for 40ns;	

	std.env.stop(0);
WAIT;                                                        
END PROCESS always;                                          
END simple_proc_arch;
