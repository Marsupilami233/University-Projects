LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab1p2map IS
port(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     LEDR: OUT STD_LOGIC_VECTor(9 downto 0) );
END lab1p2map;

ARCHITECTURE struct OF lab1p2map IS

COMPONENT simple_fib
PORT (
   x : IN std_logic_vector(3 downto 0);
   f : OUT std_logic
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : simple_fib
   PORT MAP (
	   x => sw(3 downto 0),
		f => ledr(0)
	);
	
END struct;