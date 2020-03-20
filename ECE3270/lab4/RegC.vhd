LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY RegC IS
generic (
    data_width : integer := 8
);
PORT(
   clk      :     IN std_logic; -- clock
   loadreg  :     IN std_logic; -- input enable
	addreg   :     IN std_logic; -- add enable
	shiftreg :     IN std_logic; -- shift enable
	Carry :        IN std_logic;
   D :            IN std_logic_vector(data_width downto 0);-- multiplicand
   Q :            OUT std_logic_vector(data_width downto 0); -- output shift 2	
	shift_out :    OUT  std_logic_vector(1 downto 0)

);
END RegC;

architecture behavioral of RegC is
	signal product : std_logic_vector(data_width downto 0);
BEGIN	
PROCESS (clk, loadreg, addreg, shiftreg)
	variable zero : std_logic := '0';
	variable signbit : std_logic_vector(1 downto 0);
BEGIN	
	IF (rising_edge(clk) and loadreg = '1') THEN
		product <= (others => zero);
		shift_out <= "00";
	ELSIF (rising_edge(clk) and addreg = '1')	THEN
		product <= D;
		shift_out <= D(1 downto 0);
	ELSIF (rising_edge(clk) and shiftreg = '1') THEN
		product(data_width - 2 downto 0) <= product(data_width downto 2);
		IF (Carry = '1') THEN
			signbit := "11";
		ELSIF (Carry = '0') THEN 
			signbit := "00";
		END IF;
		product(data_width downto data_width - 1) <= signbit;
	END IF;
END PROCESS;
Q <= product;	
END behavioral;