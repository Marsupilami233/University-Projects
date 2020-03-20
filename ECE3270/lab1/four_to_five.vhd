LIBRARY ieee;
USE ieee.std_logic_1164.all;

entity four_to_five is
port (
   x : IN std_logic_vector(3 downto 0);
   f : OUT std_logic_vector(4 downto 0)
);
end four_to_five;

architecture dataflow of four_to_five is
begin
    with x select
	    f <= --"10101" when "1101",
		      "01101" when "1000",
		      "01000" when "0101",
		      "00101" when "0011",
				"00011" when "0010",
				"00010" when "0001",
				"11111" when others;
end dataflow;