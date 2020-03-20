LIBRARY ieee;
USE ieee.std_logic_1164.all;

entity simple_proc is
generic(
    data_bus : integer := 8  -- need to map
);

port (
    DIN : IN std_logic_vector (data_bus  - 1 downto 0);
	 clk : IN std_logic;
	 run : IN std_logic;
	 rstn : IN std_logic;
	 bus_proc : OUT std_logic_vector (data_bus - 1 downto 0);
    done : OUT std_logic
);
end simple_proc;

architecture dataflow of simple_proc is
COMPONENT mux_vary
generic(
    data_width  : integer := 8    -- need to have actual or default value
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
END COMPONENT;

COMPONENT sim_FSM

PORT(
    Run :    IN std_logic;
	 Resetn : IN std_logic;
	 clk :    IN std_logic;
	 IRoutf : IN std_logic_vector(7 downto 0);
	 IRinf :  OUT std_logic;
	 Routf : OUT std_logic_vector(7 downto 0);
	 Rinf : OUT std_logic_vector(7 downto 0);
	 Ginf :   OUT std_logic;
	 Goutf :  OUT std_logic;
	 DINoutf :OUT std_logic;
	 Ainf :   OUT std_logic;
	 AddSubf :OUT std_logic;
	 Done :   OUT std_logic	 
);
END COMPONENT;

COMPONENT AddSub
generic(
    data_width : integer  := 8  -- need to have actual or default value
);
PORT(
    AddOrSub : IN std_logic;
	 AoutinAd : IN std_logic_vector (data_width -1 downto 0);
	 BusinAd  : IN std_logic_vector (data_width -1 downto 0);
	 AddSubout   : OUT std_logic_vector (data_width -1 downto 0) 
);
END COMPONENT;

COMPONENT Registers
generic(
    data_width  : integer := 8   -- need to have actual or default value 
);
PORT(
   Input_select : IN std_logic;
   D :            IN std_logic_vector(data_width - 1 downto 0);
   clk :          IN std_logic;
   Output :       OUT std_logic_vector(data_width - 1 downto 0)	
);
END COMPONENT;

COMPONENT IR
PORT(
   Input_select : IN std_logic;
   D :            IN std_logic_vector(7 downto 0);
   clk :          IN std_logic;
   Output :       OUT std_logic_vector(7 downto 0)	
);
END COMPONENT;

SIGNAL  IR_out: std_logic_vector(7 downto 0);
SIGNAL  mux_out : std_logic_vector(data_bus  - 1 downto 0);
SIGNAL  R0,R1,R2,R3,R4,R5,R6,R7,G : std_logic_vector(data_bus - 1 downto 0);-- outputs
SIGNAL  Rin_sig, Rout_sig : std_logic_vector(7 downto 0);
SIGNAL  Ain_sig, Add_sig, Gin_sig, Gout_sig, DINout_sig, IRin_sig : std_logic;
SIGNAL  A_to_Add, Add_to_G: std_logic_vector(data_bus  - 1 downto 0);
begin
    -- create instantiations of Registers  R0-R7
    regR0 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(0),D => mux_out ,clk=>clk,Output=>R0);
    regR1 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(1),D => mux_out ,clk=>clk,Output=>R1);
    regR2 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(2),D => mux_out ,clk=>clk,Output=>R2);
    regR3 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(3),D => mux_out ,clk=>clk,Output=>R3);
    regR4 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(4),D => mux_out ,clk=>clk,Output=>R4);
    regR5 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(5),D => mux_out ,clk=>clk,Output=>R5);
    regR6 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(6),D => mux_out ,clk=>clk,Output=>R6);
    regR7 : Registers  generic map (data_bus ) PORT MAP(Input_Select => Rin_sig(7),D => mux_out ,clk=>clk,Output=>R7);
    -- These are the internal signal but not the newly declared ports!!!!!!
    regA  : Registers  generic map (data_bus ) PORT MAP(Input_Select => Ain_sig,D => mux_out,clk=>clk,Output=> A_to_Add);
    regG  : Registers  generic map (data_bus ) PORT MAP(Input_Select => Gin_sig,D => Add_to_G,clk=>clk,Output=> G); 

    -- source citation of the Registers found in the slide

    P1_multiplexers : mux_vary
	 PORT MAP(R0,R1,R2,R3,R4,R5,R6,R7,DIN,G,Gout_sig,DINout_sig,Rout_sig(0),Rout_sig(1),
	          Rout_sig(2),Rout_sig(3),Rout_sig(4),Rout_sig(5),Rout_sig(6),Rout_sig(7),mux_out);
	 P2_FSM : sim_FSM
    PORT MAP(run,rstn,clk,IR_out,IRin_sig,Rout_sig,Rin_sig,Gin_sig,Gout_sig, DINout_sig, Ain_sig, Add_sig, done);

	 P3_IR : IR
	 PORT MAP(IRin_sig, DIN(7 downto 0), clk, IR_out); 
	 
    P4_AddSub : AddSub
	 PORT MAP (Add_sig, A_to_Add, mux_out, Add_to_G);
    -----
    bus_proc <= mux_out;	 
	 
end dataflow;