LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_arith.all;
-- User input bits to the  B/2
-- and after that, read only from 
-- count
ENTITY ShiftRegD IS
generic (
    data_width : integer := 8
);
PORT(
   loadreg :      IN std_logic; -- input enable
   clk :          IN std_logic;
	count :        IN std_logic;-- make it binary
   Q :       OUT std_logic_vector(2 downto 0) -- output	
);
END ShiftRegD;

architecture behavioral of ShiftRegD is
signal count_number : integer ;
signal count_vector : std_logic_vector(2 downto 0) ;
BEGIN
    
    PROCESS (clk, loadreg, count)
	 BEGIN
	 IF rising_edge(clk) and loadreg ='1' THEN
       count_number <= data_width/2;
    ELSIF rising_edge(clk) and count = '1' THEN	
       count_number <= count_number - 1;	 
	 END IF;	 
	 END PROCESS;
	 count_vector <= conv_std_logic_vector(count_number, 3);
	 Q <= count_vector;
END behavioral;