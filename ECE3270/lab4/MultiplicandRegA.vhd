LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
ENTITY MultiplicandRegA IS
generic (
    data_width : integer := 8
);
PORT(
   loadreg :      IN std_logic; -- input enable
   D :            IN std_logic_vector(data_width - 1 downto 0);-- multiplicand
   clk :          IN std_logic;
   Q2 :           OUT std_logic_vector(data_width downto 0); -- output shift 2	
	Q_2:           OUT std_logic_vector(data_width downto 0);-- output	shift + 2's compliment
	Q1 :           OUT std_logic_vector(data_width downto 0); -- output D
	Q_1:           OUT std_logic_vector(data_width downto 0) -- output  2's compliment
);
END MultiplicandRegA ;

architecture behavioral of MultiplicandRegA  is
BEGIN
    PROCESS (clk, loadreg) 
	 BEGIN
	     IF rising_edge(clk) and loadreg = '1' THEN 	         
			   Q2  <= D & '0';  
		      Q_2 <= (NOT(D) + '1') & '0';
				Q1  <= '0' & D;
				--Q_1 <= NOT(D + '1');
				IF D(data_width - 1) = '0' THEN
				    Q_1 <= '1' & (NOT(D) + '1');
				ELSIF D(data_width - 1) = '1' THEN
				    Q_1 <= '0' & (NOT(D) + '1');
			   END IF;
	     END IF;
	 END PROCESS;
END behavioral;