LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY lab4map IS
generic(
    data_bus : integer := 4  -- need to map
);
PORT(SW: IN STD_LOGIC_VECTOR(9 downto 0);
     KEY: IN STD_LOGIC_VECTOR(3 downto 0);
	  LEDR: OUT STD_LOGIC_VECTOR(9 downto 0));
END lab4map;

ARCHITECTURE struct OF lab4map IS

COMPONENT Recoded
generic(
    data_bus : integer := 4  -- need to map to the broad
);
PORT(
    Start :        IN std_logic;
	 Resetn :       IN std_logic;
	 clk :          IN std_logic;
	 Multiplicand : IN std_logic_vector(data_bus - 1 downto 0);
	 Multiplier :   IN std_logic_vector(data_bus - 1 downto 0);
	 ProductH :     OUT std_logic_vector(data_bus - 1 downto 0); 
	 ProductL :     OUT std_logic_vector(data_bus - 1 downto 0); 
	 Busy :         OUT std_logic;
	 Done :         OUT std_logic	 
);
END COMPONENT;

BEGIN
   --Create the device and map it to the appropriate pins
struct : Recoded
   PORT MAP (
	
		clk => key(0),
		Start => sw(8),
		Resetn => sw(9),
		Multiplicand => sw(7 downto 4),
		Multiplier => sw(3 downto 0),
		done => ledr(9),
		Busy => ledr(8),
		ProductH => ledr(7 downto 4),
		ProductL => ledr(3 downto 0)
	
	);
	
END struct;