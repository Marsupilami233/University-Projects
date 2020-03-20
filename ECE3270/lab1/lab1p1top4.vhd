LIBRARY ieee;
USE ieee.std_logic_1164.all;
---Declare the very beginning and very end (terminals)
entity lab1p1top4 is
port (
   input4 :      IN std_logic_vector(3 downto 0);
	select_next : IN std_logic;
   output7 :     OUT std_logic_vector(6 downto 0);
	valid_fib :   OUT std_logic
);
end lab1p1top4;

architecture mixed of lab1p1top4 is
component  mux_2_to_1
   port (
      x : IN std_logic_vector(3 downto 0);---input
      y : IN std_logic_vector(3 downto 0);---next 
      s : IN std_logic;                   ---triger
      m : OUT std_logic_vector(3 downto 0)---the output may be input or next  
	);
end component;

component simple_fib
   port (
	   x : IN std_logic_vector(3 downto 0);--- the output from the x of the mux_2_to_1
	   f : OUT std_logic---the output for the valid input from x
	);
end component;

component four_to_five
   port (
	   x : IN std_logic_vector(3 downto 0);--- the input from the x of mux_2_to_1
		f : OUT std_logic_vector(4 downto 0) --- the output of the next fib
	);
end component;
	
component seven_seg_decoder
   port (
      x : IN std_logic_vector(3 downto 0);--- the final output from the four_to_five
      f : OUT std_logic_vector(6 downto 0)--- the output needs to map to the HEX0
	);
end component;
-- Set the signals for the bits transactions
signal output_next4 : std_logic_vector(4 downto 0);
signal mux_out : std_logic_vector(3 downto 0);-- should map or you will lost the signal
begin
   p2_simple_fib: simple_fib
	port map (input4,valid_fib);
   p3_4_to_5 : four_to_five 
	port map (input4,output_next4);	
	p1_mux_2_to_1 : mux_2_to_1
   port map (input4,output_next4(3 downto 0),select_next,mux_out);	
	p4_seven_seg_decoder : seven_seg_decoder
   port map (mux_out, output7);	
end mixed;

