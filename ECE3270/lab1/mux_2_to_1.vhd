LIBRARY ieee;
USE ieee.std_logic_1164.all;

entity mux_2_to_1 is
port (
   x : IN std_logic_vector(3 downto 0);
   y : IN std_logic_vector(3 downto 0);
   s : IN std_logic;
   m : OUT std_logic_vector(3 downto 0)
);
end mux_2_to_1;

architecture dataflow of mux_2_to_1 is
begin
    with s select
	 m <= x when '0',
	      y when others;
end dataflow;