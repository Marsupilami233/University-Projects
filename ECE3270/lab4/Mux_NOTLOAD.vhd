LIBRARY ieee;
USE ieee.std_logic_1164.all;
--USE ieee.numeric_std.all;
-- This 2-bit multiplexer is the 
-- When only the loadreg is low add and shift, add and shift
ENTITY Mux_NOTLOAD IS
generic (
    data_width : integer := 8
);
PORT(
   NOTloadreg : IN std_logic; -- input enable
   D :          IN std_logic_vector(data_width downto 0);-- results from ALU
   Q :          OUT std_logic_vector(data_width downto 0) -- output	
);
END Mux_NOTLOAD;

architecture behavioral of Mux_NOTLOAD is

BEGIN
   with NOTloadreg select 
	     Q <= D when '0',
		  (others => '0')  when others;
END behavioral;