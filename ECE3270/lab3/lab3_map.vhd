LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab3_map IS
generic(
    data_bus : integer := 8  -- need to map
);
PORT(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     KEY: IN STD_LOGIC_VECTOR(3 downto 0);
	  LEDR: OUT STD_LOGIC_VECTOR(9 downto 0));
END lab3_map;

ARCHITECTURE struct OF lab3_map IS

COMPONENT simple_proc

PORT (
    DIN : IN std_logic_vector (data_bus - 1 downto 0);
	 clk : IN std_logic;
	 run : IN std_logic;
	 rstn : IN std_logic;
	 bus_proc : OUT std_logic_vector (data_bus - 1 downto 0);
    done : OUT std_logic
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : simple_proc
   PORT MAP (
	
		clk => key(0),
		run => key(1),
		rstn => sw(9),
		DIN => sw(7 downto 0),
		bus_proc => ledr(7 downto 0),
		done => ledr(9)
	
	);
	
END struct;