LIBRARY ieee;
USE ieee.std_logic_1164.all;

entity seven_seg_decoder is
port (
   x : IN std_logic_vector(3 downto 0);
   f : OUT std_logic_vector(6 downto 0)
);
end seven_seg_decoder ;

architecture dataflow of seven_seg_decoder is
begin
    with x select
	    f <= "1000000" when "0000",
		      "1111001" when "0001",
		      "0100100" when "0010",
		      "0110000" when "0011",
				"0010010" when "0101",
				"0000000" when "1000",
				"0000110" when others;
end dataflow;
