LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY mux_vary IS
GENERIC(
    data_width : integer := 16    
);
PORT(
   R0out,R1out,R2out,R3out,R4out,R5out,R6out,R7out : IN std_logic_vector(data_width-1 downto 0);
   DINout, Gout : IN std_logic_vector(data_width-1 downto 0);
	Gout_select , DINout_select : IN std_logic;
   R0out_select, R1out_select,R2out_select, 
	R3out_select,R4out_select,R5out_select,
	R6out_select, R7out_select : IN std_logic;
   Bus_out : OUT std_logic_vector(data_width-1 downto 0)	
);
END mux_vary;

architecture dataflow of mux_vary is
begin 
	   Bus_out <= R0out when (R0out_select = '1') else
              R1out when (R1out_select = '1') else
              R2out when (R2out_select = '1') else
              R3out when (R3out_select = '1') else
              R4out when (R4out_select = '1') else
              R5out when (R5out_select = '1') else
              R6out when (R6out_select = '1') else	
              R7out when (R7out_select = '1') else
				  DINout when (DINout_select = '1') else
	           Gout when (Gout_select = '1') else DINout;			  
end dataflow;
