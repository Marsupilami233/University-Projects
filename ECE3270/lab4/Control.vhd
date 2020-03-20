LIBRARY ieee;
USE ieee.std_logic_1164.all;
--USE ieee.numeric_std.all;
--use IEEE.STD_LOGIC_UNSIGNED.ALL;
ENTITY Control IS

PORT(
    Start :   IN std_logic;
	 Resetn :  IN std_logic;
	 clk :     IN std_logic;
	 RegD :    IN std_logic_vector(2 downto 0);
	 Loadreg : OUT std_logic;
	 Shiftreg :OUT std_logic;
	 Count :   OUT std_logic;
	 Addreg :  OUT std_logic;
	 Busy   :  OUT std_logic;
	 Done :    OUT std_logic	 
);
END Control;

architecture behavioral of Control is
-- the meanly machine
type states is (NotBusy, Load, Add, ShiftTwo);
-- the state changes in the process, we have to use a different process to handle that 
-- the state update only then the update reset and transition happens and the input staste changes
signal current_state : states;
-- first the process do the transistion
-- second process update the output 

begin
   PROCESS (clk, Resetn)
   BEGIN
        IF (Resetn = '0') THEN
            current_state  <= NotBusy; --Upon reset, set the state to the NotBusy
        ELSIF rising_edge(clk) THEN    --IF there is a rising edge of clock, then power on the state machine and do the transition
        CASE current_state IS --The State Machine assign an value into the instruction register, grap value from DIN and on a high "Run" signal and begin
	-- First state : NotBusy : the machine begin when the START signal becomes high BUSY signal becomes low (busy and done set by state machine) 		    
		    WHEN NotBusy => 
		        IF Start = '1' THEN 
		        current_state <= Load;
              ELSE 
			     current_state <= NotBusy;
		        END IF;
	-- Second state : Load : the machine performs the task of loading the multiplicand, multiplexer, B/2 and PRODUCT to RegA, RegB, RegD, RegC    
        -- Start should be 0, Busy signal should be 1, Shiftreg should be 0, Count should be assigned with the binary value of half data_width, 
        -- Loadreg should be set to 0, Addreg should be set to 0  	    
          WHEN Load =>
              current_state <= Add;
        -- Third state : Add :  the machine performs the task of decrementing the count, set the Addreg to 1, any other signals except busy must be 0				  
	       WHEN Add =>
			     current_state <= ShiftTwo;
        -- Fourth state : Shift : the machine performs the shift, only the shift will be 1 and busy should be one, any other signals except busy must be 0
		    WHEN ShiftTwo =>
			     IF RegD = "000" THEN
			     current_state <= NotBusy;
			     ELSE	 
			     current_state <= Add;	  
		        END IF;
        -- Fifth state : Finish : the machine finishes, so the busy bit will be 0, done will be 1, and needs to be reset to the initial state     		  
          END CASE;
        END IF;		
    END PROCESS;
	 -- generate the output 

	 PROCESS(current_state, RegD) 
	 BEGIN
	    CASE current_state IS
		    WHEN NotBusy =>
			 Loadreg <= '0';
			 Shiftreg <= '0';
--			 BitPair <= "000";
			 Count <= '0';
		    Addreg <= '0';
		    Busy   <= '0';
          Done   <= '0';
          WHEN Load => 
		    Loadreg <= '1';
		    Shiftreg <= '0';
--		    BitPair  <= "000";
          Count  <= '0';
			 Addreg <= '0';
			 Busy   <= '1';
			 Done   <= '0';
		    WHEN Add =>
		    Loadreg <= '0';
		    Shiftreg <= '0';
--		    BitPair <= Shift;
			 Count   <= '0';
		    Addreg  <= '1';
			 Busy    <= '1';
			 Done    <= '0';
		    WHEN ShiftTwo =>
		    Loadreg <= '0';
		    Shiftreg <= '1';
--		    BitPair <= Shift;
			 Count   <= '1'; 
		    Addreg  <= '0';
			 Busy    <= '1';
			 Done    <= '0';
			 IF RegD = "000" THEN
			    Done <= '1';
			 ELSE 
			    Done <= '0'; 
			 END IF;	 
		  END CASE;
    END PROCESS;

END behavioral;	 
