LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab2mapping IS
PORT(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     KEY: IN STD_LOGIC_VECTOR(3 downto 0);
	  LEDR: OUT STD_LOGIC_VECTOR(9 downto 0));
END lab2mapping;

ARCHITECTURE struct OF lab2mapping IS

COMPONENT airplane
PORT (
   clk : IN std_logic; -- rising edge triggered
	reset : IN std_logic; --active low
	ivalid : IN std_logic; -- ivalid --> if the input is valid
	smooth : IN std_logic; -- smooth -->  if the plane is stable
	alt_10k : IN std_logic; -- alt_10k if the plane is flying through 10k
	alt_25k : IN std_logic; -- alt_25k if the plane is flying through 25k
	seatbelt : OUT std_logic; -- passeger needs to wear seatbelt
	noelectronics : OUT std_logic -- no electronic is allowed
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : airplane
   PORT MAP (
	
		clk => key(0),
		reset => sw(9),
		ivalid => sw(8),
		smooth => sw(2),
		alt_10k => sw(1),
		alt_25k => sw(0),
		seatbelt => ledr(0),
		noelectronics => ledr(1)
	
	);
	
END struct;