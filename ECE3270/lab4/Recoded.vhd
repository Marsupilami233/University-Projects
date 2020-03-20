LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY Recoded IS
generic(
    data_bus : integer := 8  -- need to map to the broad
);
PORT(
    Start :        IN std_logic;
	 Resetn :       IN std_logic;
	 clk :          IN std_logic;
	 Multiplicand : IN std_logic_vector(data_bus - 1 downto 0);
	 Multiplier :   IN std_logic_vector(data_bus - 1 downto 0);
	 ProductH :     OUT std_logic_vector(data_bus - 1 downto 0); 
	 ProductL :     OUT std_logic_vector(data_bus - 1 downto 0); 
	 Busy :         OUT std_logic;
	 Done :         OUT std_logic	 
);
END Recoded;

architecture dataflow of Recoded is
COMPONENT MultiplicandRegA 
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
END COMPONENT;

COMPONENT RegB
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
   bitpair  :     OUT std_logic_vector(2 downto 0)
);
END COMPONENT;

COMPONENT RegC
generic (
    data_width : integer := 8
);
PORT(
   clk      :     IN std_logic; -- clock
   loadreg  :     IN std_logic; -- input enable
	addreg   :     IN std_logic; -- add enable
	shiftreg :     IN std_logic; -- shift enable
	Carry    :     IN std_logic; -- Carry bit
   D :            IN std_logic_vector(data_width downto 0);-- multiplicand
   Q :            OUT std_logic_vector(data_width downto 0); -- output shift 2	
	shift_out:     OUT std_logic_vector(1 downto 0) -- RegC shift to regsters B
);
END COMPONENT;

COMPONENT Mux_CAND
generic (
    data_width : integer := 8
);
PORT(
   BIT_WISE         : IN std_logic_vector(2 downto 0); -- input from the RegB and the state machine
   Q1,Q2,Q_1,Q_2    : IN std_logic_vector(data_width downto 0);-- result from RegA
   OUTPUT           : OUT std_logic_vector(data_width downto 0) -- output	
);
END COMPONENT;

COMPONENT Adder 
generic(
    data_width : integer := 8    
);
PORT(
	 Datain      : IN std_logic_vector (data_width  downto 0);  -- data input
	 RegCin      : IN std_logic_vector (data_width  downto 0);  -- Register C input
	 AddResult   : OUT std_logic_vector (data_width downto 0); -- result 9 bit part
	 Carry       : OUT std_logic -- A bit that needs to carry out	 
);
END COMPONENT;

COMPONENT Mux_NOTLOAD
generic (
    data_width : integer := 8
);
PORT(
   NOTloadreg : IN std_logic; -- input enable
   D :          IN std_logic_vector(data_width downto 0);-- results from ALU
   Q :          OUT std_logic_vector(data_width downto 0) -- output	
);
END COMPONENT;

COMPONENT ShiftRegD
generic (
    data_width : integer := 8
);
PORT(
   loadreg :      IN std_logic; -- input enable
   clk :          IN std_logic;
	count :        IN std_logic;-- make it binary
   Q :       OUT std_logic_vector(2 downto 0) -- output	
);
END COMPONENT;

COMPONENT Control

PORT(
    Start :   IN std_logic;
	 Resetn :  IN std_logic;
	 clk :     IN std_logic;
	 RegD :    IN std_logic_vector(2 downto 0);
--	 Shift :   IN std_logic_vector(2 downto 0);
--	 BitPair:  OUT std_logic_vector(2 downto 0); 
	 Loadreg : OUT std_logic;
	 Shiftreg :OUT std_logic;
	 Count :   OUT std_logic;
	 Addreg :  OUT std_logic;
	 Busy   :  OUT std_logic;
	 Done :    OUT std_logic	 
);
END COMPONENT;


SIGNAL  loadreg, shiftreg, count_enable, addreg : std_logic;
SIGNAL  Q1, Q_1, Q2, Q_2 : std_logic_vector(data_bus downto 0);
SIGNAL  Part_C, NOT_OUT : std_logic_vector(data_bus downto 0);-- revised 
SIGNAL  Prod_A ,Product : std_logic_vector(data_bus downto 0); -- revised
SIGNAL  Part_B : std_logic_vector(data_bus - 1 downto 0);
--SIGNAL  SHIFT, COUNT, BITPAIRSIG : std_logic_vector(2 downto 0);
SIGNAL  COUNT,BITPAIRSIG : std_logic_vector(2 downto 0);
SIGNAL  Shift_inout  : std_logic_vector(1 downto 0);
SIGNAL  Carry_in  : std_logic;
begin

    P1_RegA : MultiplicandRegA
	 GENERIC MAP (data_bus)
	 PORT MAP (loadreg, Multiplicand, clk,Q2, Q_2, Q1, Q_1);
	 
    P2_RegB : RegB
	 GENERIC MAP (data_bus)
	 PORT MAP (clk,loadreg, shiftreg,Shift_inout, Multiplier, Part_B, BITPAIRSIG);
	 
    P3_RegC : RegC
	 GENERIC MAP (data_bus)
	 PORT MAP (clk,loadreg,addreg,shiftreg,Carry_in ,NOT_OUT,Part_C, Shift_inout); -- changed everythings  with the carry bit
------------------------------------------------------
    P4_Mux_CAND : Mux_CAND
	 GENERIC MAP (data_bus)
	 PORT MAP (BITPAIRSIG,Q1,Q2,Q_1,Q_2,Prod_A);
	 
    P5_Adder : Adder
	 GENERIC MAP (data_bus)	 
	 PORT MAP (Prod_A,Part_C,Product, Carry_in);

    P6_Mux_NOTLOAD : Mux_NOTLOAD
	 GENERIC MAP (data_bus)	 
	 PORT MAP (loadreg,Product,NOT_OUT);

    P7_ShiftRegD : ShiftRegD
	 GENERIC MAP (data_bus)	 
	 PORT MAP (loadreg,clk,count_enable,COUNT); 
	 
    P8_Control : Control

	 PORT MAP (Start,Resetn,clk,COUNT,
	 loadreg,shiftreg,count_enable,addreg,Busy,Done);	 

    --------------------------------------
    ProductH <= Part_C(data_bus - 1 downto 0);
	 ProductL <= Part_B;
	 
end dataflow;