LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab1p4map IS
port(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     HEX0: OUT STD_LOGIC_VECTOR(6 downto 0));
END lab1p4map;

ARCHITECTURE struct OF lab1p4map IS

COMPONENT seven_seg_decoder
PORT (
   x : IN std_logic_vector(3 downto 0);
   f : OUT std_logic_vector(6 downto 0)
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : seven_seg_decoder
   PORT MAP (
	   x => sw(3 downto 0),
		f => hex0(6 downto 0)
	);
	
END struct;