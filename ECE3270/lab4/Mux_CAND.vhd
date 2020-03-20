LIBRARY ieee;
USE ieee.std_logic_1164.all;
--USE ieee.numeric_std.all;
-- This 2-bit multiplexer is the 
-- When only the loadreg is low add and shift, add and shift
ENTITY Mux_CAND IS
generic (
    data_width : integer := 8
);
PORT(
   BIT_WISE         : IN std_logic_vector(2 downto 0); -- input from the RegB and the state machine
   Q1,Q2,Q_1,Q_2    : IN std_logic_vector(data_width downto 0);-- result from RegA
   OUTPUT           : OUT std_logic_vector(data_width downto 0) -- output	
);
END Mux_CAND;

architecture behavioral of Mux_CAND is

BEGIN
   with BIT_WISE select 
	     OUTPUT <= 
		 (others => '0')    when "000",
		      Q1         when "001",
				Q1         when "010",
				Q2         when "011",
				Q_2        when "100",
				Q_1        when "101",
				Q_1        when "110",
		 (others => '0')    when "111",
		  (others => 'U') when others;
END behavioral;