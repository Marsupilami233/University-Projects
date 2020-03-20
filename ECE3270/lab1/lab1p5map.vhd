LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab1p5map IS
port(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     HEX0: OUT STD_LOGIC_VECTOR(6 downto 0);
	  LEDR: OUT STD_LOGIC_VECTOR(9 downto 0));
END lab1p5map;

ARCHITECTURE struct OF lab1p5map IS

COMPONENT lab1p1top4
PORT (
   input4 :      IN std_logic_vector(3 downto 0);
	select_next : IN std_logic;
   output7 :     OUT std_logic_vector(6 downto 0);
	valid_fib :   OUT std_logic
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : lab1p1top4
   PORT MAP (
	   input4 => sw(3 downto 0),
		select_next => sw(9),
		output7 => hex0(6 downto 0),
		valid_fib => LEDR(0)
	);
	
END struct;