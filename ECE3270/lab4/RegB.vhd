LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY RegB IS
generic (
    data_width : integer := 8
);
PORT(
   clk      :     IN std_logic; -- clock
   loadreg  :     IN std_logic; -- input enable
	shiftreg :     IN std_logic; -- shift enable
	shiftin  :     IN std_logic_vector(1 downto 0); -- shift enable
   D :            IN std_logic_vector(data_width - 1 downto 0);-- multiplicand
   Q :            OUT std_logic_vector(data_width - 1 downto 0); -- output shift 2	
   bitpair  :    OUT std_logic_vector(2 downto 0)
);
END RegB;

architecture behavioral of RegB is
signal product : std_logic_vector(data_width - 1 downto 0);
signal bitpairs: std_logic_vector(2 downto 0);
BEGIN
   PROCESS (clk, loadreg, shiftreg)
	BEGIN
      IF (loadreg = '1' and rising_edge(clk)) THEN 
		   product <= D;
			bitpairs <= D(1 downto 0) & '0';
		ELSIF (shiftreg = '1' and rising_edge(clk))THEN
			product(data_width - 3 downto 0) <= product(data_width - 1 downto 2);
			product(data_width - 1 downto data_width - 2) <= shiftin;
			bitpairs <= product(3 downto 1);
		END IF;	   
   END PROCESS;	
Q <= 	product;
bitpair <= bitpairs;
END behavioral;