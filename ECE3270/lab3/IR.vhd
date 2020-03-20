LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY IR IS
PORT(
   Input_select : IN std_logic;
   D :            IN std_logic_vector(7 downto 0);
   clk :          IN std_logic;
   Output :       OUT std_logic_vector(7 downto 0)	
);
END IR;

architecture behavioral of IR is
BEGIN
    PROCESS (clk, Input_select) 
	 BEGIN
	     IF rising_edge(clk) and Input_select = '1' THEN 
	         Output <= D;
	     END IF;
	 END PROCESS;
END behavioral;
