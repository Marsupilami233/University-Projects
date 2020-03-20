LIBRARY ieee;
USE ieee.std_logic_1164.all;


ENTITY airplane_cl is
	PORT(clock, resetn, ivalid, iready: IN std_logic;
		  datain: IN std_logic_vector(7 downto 0);
		  oready, ovalid: OUT std_logic;
		  dataout: OUT std_logic_vector(7 downto 0));
END airplane_cl;

ARCHITECTURE structure of airplane_cl is
	--Declare the component, then port map it!
COMPONENT airplane IS
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
-- port mapping input from the output from entity
struct : airplane
   PORT MAP(
      clk => clock,
      reset => resetn,
      ivalid => ivalid,
      alt_10k => datain(2),
      alt_25k => datain(1),
      smooth => datain(0),
      seatbelt => dataout(1),
      noelectronics => dataout(0)
   );

END structure;
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	
	
	
