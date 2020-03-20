LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab1p3map IS
port(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     LEDR: OUT STD_LOGIC_VECTOR(9 downto 0));
END lab1p3map;

ARCHITECTURE struct OF lab1p3map IS

COMPONENT four_to_five
PORT (
   x : IN std_logic_vector(3 downto 0);
   f : OUT std_logic_vector(4 downto 0)
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : four_to_five
   PORT MAP (
	   x => sw(3 downto 0),
		f => ledr(4 downto 0)
	);
	
END struct;
