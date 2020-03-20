LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.numeric_std.all;
USE ieee.std_logic_unsigned.all;
--- Cited from the educational website:https://www.csee.umbc.edu/portal/help/VHDL/stdpkg.html
--- Or the source could be found on the textbook Page 144
ENTITY AddSub IS
generic(
    data_width : integer := 16
    
);
PORT(
    AddOrSub : IN std_logic;
	 AoutinAd : IN std_logic_vector (data_width -1 downto 0);
	 BusinAd  : IN std_logic_vector (data_width -1 downto 0);
	 AddSubout   : OUT std_logic_vector (data_width -1 downto 0) 
);
END AddSub;
-- first read the data that is stored in one of the As and store it in RXin
-- second AddSub = 1 sub, or add!!!!!
architecture behavioral of AddSub is
BEGIN
    AddSubout <= (AoutinAd + BusinAd) when (AddOrSub = '0') else 
                (AoutinAd - BusinAd);	 
END behavioral;