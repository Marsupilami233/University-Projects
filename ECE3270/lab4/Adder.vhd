LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
--- Cited from the educational website:https://www.csee.umbc.edu/portal/help/VHDL/stdpkg.html
--- Or the source could be found on the textbook Page 144
ENTITY Adder IS
generic(
    data_width : integer := 8    
);
PORT(
	 Datain      : IN std_logic_vector (data_width  downto 0);  -- data input
	 RegCin      : IN std_logic_vector (data_width downto 0);  -- Register C input
	 AddResult   : OUT std_logic_vector (data_width downto 0); -- result 9 bit part
	 Carry       : OUT std_logic -- A bit that needs to carry out
);
END Adder;

architecture behavioral of Adder is
signal result : std_logic_vector(data_width + 1 downto 0);
BEGIN
    result <= (Datain(data_width) & Datain) + (RegCin(data_width) & RegCin);
	 AddResult <= result(data_width downto 0);
	 Carry  <= result(data_width + 1);
END behavioral;