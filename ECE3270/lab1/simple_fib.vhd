LIBRARY ieee;
USE ieee.std_logic_1164.all;

entity simple_fib is 
port ( 
    x : IN std_logic_vector(3 downto 0);
	 f : OUT std_logic
);
end simple_fib;

architecture fib4bit of simple_fib is
begin
    with x select 
		f <= '1' when "0000" | "0001" | "0010" | "0011" | "0101" | "1000" | "1101" ,
	        '0' when others; 
			  
	--f <= '1' when s="0000" | s="0001" ....| s="1101" else '0';
end fib4bit;
