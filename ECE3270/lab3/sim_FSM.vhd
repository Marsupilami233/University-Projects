LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY sim_FSM IS

PORT(
         Run :    IN std_logic;
	 Resetn : IN std_logic;
	 clk :    IN std_logic;
	 IRoutf : IN std_logic_vector(7 downto 0);
	 IRinf :  OUT std_logic;
	 Routf:   OUT std_logic_vector(7 downto 0);
	 Rinf :   OUT std_logic_vector(7 downto 0);
	 Ginf :   OUT std_logic;
	 Goutf :  OUT std_logic;
	 DINoutf :OUT std_logic;
	 Ainf :   OUT std_logic;
	 AddSubf :OUT std_logic;
	 Done :   OUT std_logic	 
);
END sim_FSM;

architecture behavioral of sim_FSM is
-- the meanly machine
type states is (T0, T1, T2, T3);
-- the state changes in the process, we have to use a different process to handle that 
-- the state update only then the update reset and transition happens and the input staste changes
signal current_state : states;
-- first the process do the transistion
-- second process update the output 
begin
   PROCESS (clk, Resetn)
   BEGIN
        IF (Resetn = '0') THEN
            current_state  <= T0; --Upon reset, set the state to the initial state: to the T0 and select the default signal as 
        ELSIF rising_edge(clk) THEN --if there is a rising edge of clock, then power on the state machine and do the transition
            CASE current_state IS --The State machine assign an value into the instruction register, grap value from DIN and on a high "Run" signal and begin
	-- First state : T0 the machine assigns default input values and then reads the instructions on the IR and ready to calculate		    
		    WHEN T0 => 
		        IF Run = '1' THEN 
				  current_state <= T1;
              ELSE 
			     current_state <= T0;
				  END IF;
	-- Second state : T1 the machine performs the task of mv Rx,Ry and mvi Rx,D then grap the value from DIN (its on the bus), update the value in R0 by
        -- set the R0in High, and choose the Multiplexer input_select: R0out, then the bus has that R0y value and open up other register R1x and the value
        -- is kept in that register R1. Assume that these trasitions will complete in one clock cycle   	    
          WHEN T1 =>
		        IF IRoutf(7) = '0'  THEN 
				  current_state <= T0;
				  ELSE
				  current_state <= T2;
				  END IF;
        -- Third state : T2 the machine performs the task of add Rx, Ry	but first fetch the output from the Rx registers (Rxout) and store it in the Ain register by
        -- setting the bits to high, and then fetch the output from the other Ry registers (Ryout) and the Ryout is available on the bus line. On the next clock cycle,
        -- Ain's value (value in Rx register) will do the add calculation and then go to G register by set Gin_select high. Finally, store the Gout to the Rxin.			  
			 WHEN T2 =>
			          current_state <= T3;
        -- Fourth state : T3 the machine performs the task of add Rx, Ry but first fetch the output from the Rx registers (Rxout) and store it in the Ain register by
        -- setting the bits to high, and then fetch the output from the other Ry registers (Ryout) and the Ryout is available on the bus line. On the next clock cycle,
        -- Ain's value (value in Rx register) will do the substraction calculation and then go to G register by set Gin_select high. Finally, store the Gout to the Rxin.			 				  
			 WHEN T3 =>
			          current_state <= T0;	  
          END CASE;
		  END IF;		
    END PROCESS; 
       -- To prevent the unwanted states to emerge, when it is T0, the instruction is allowed to add but if the calculatino process is going on, the instruction will
       -- not be inputed
    PROCESS (current_state, IRoutf)
       -- As an meanly machine, we need to set up the output based on our current_state and IRoutf
    BEGIN 
        CASE current_state IS
	     WHEN T0 =>
		      IRinf <= '1';-- Make it start transition When completing mvi, the value to be stored in RX 
                             -- must be on the DIN pins by the next active clock cycle after storing instruction
                             -- in the IR register
                Routf <= "00000000";
                Goutf <= '0';                
                DINoutf <= '0';
                Rinf <= "00000000";
                Ainf <= '0';
                AddSubf <= '0';
                Ginf <= '0';
                Done <= '0';
	     WHEN T1 => --- The difference between those states is that the first two states will soon go back to the starting state, the 
		             --- Input signals will not have to be reset, but for the other states like add and sub, the T1, T2, T3 will not
						 --- Go back to the current state, so that every state transitions happens, the signals needs to be reseted.
		      IRinf <= '0';
                Routf <= "00000000";
                Goutf <= '0';                
                DINoutf <= '0';
                Rinf <= "00000000";
                Ainf <= '0';
                AddSubf <= '0';
                Ginf <= '0';
                Done <= '0';				
                IF (IRoutf(7 downto 6) = "00") THEN -- mv Rx, Ry; Done;
                    -- First check for YYY and based on the YYY -> select the register to go on the Bus
                    IF (IRoutf(2 downto 0) = "000") THEN Routf(0) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "001") THEN Routf(1) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "010") THEN Routf(2) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "011") THEN Routf(3) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "100") THEN Routf(4) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "101") THEN Routf(5) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "110") THEN Routf(6) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "111") THEN Routf(7) <= '1';
                    END IF;
                    -- Second check for XXX and based on the XXX -> select the register to store data from BUS 
                    IF (IRoutf(5 downto 3) = "000") THEN Rinf(0) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "001") THEN Rinf(1) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "010") THEN Rinf(2) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "011") THEN Rinf(3) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "100") THEN Rinf(4) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "101") THEN Rinf(5) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "110") THEN Rinf(6) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "111") THEN Rinf(7) <= '1';
                    END IF;
                Done <= '1';
                ELSIF (IRoutf(7 downto 6) = "01") THEN -- mvi Rx, #D; Done;
                    DINoutf <= '1'; -- fetch the value from the DIN	
                    -- Check for XXX and based on the XXX -> select the register to store Data from BUS
                    IF (IRoutf(5 downto 3) = "000") THEN Rinf(0) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "001") THEN Rinf(1) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "010") THEN Rinf(2) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "011") THEN Rinf(3) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "100") THEN Rinf(4) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "101") THEN Rinf(5) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "110") THEN Rinf(6) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "111") THEN Rinf(7) <= '1';
                    END IF;
                Done <= '1';
                -- add sub procedure
                ELSIF (IRoutf(7 downto 6) = "10")  THEN -- add/sub Rx, Ry; 
                    -- RXout, Ain
                    -- Check for XXX and based on the XXX -> select the register to output data to BUS
                    IF (IRoutf(5 downto 3) = "000") THEN Routf(0) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "001") THEN Routf(1) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "010") THEN Routf(2) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "011") THEN Routf(3) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "100") THEN Routf(4) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "101") THEN Routf(5) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "110") THEN Routf(6) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "111") THEN Routf(7) <= '1';
                    END IF;
                    Ainf <= '1'; -- select the register to import data to Ain
                ELSIF (IRoutf(7 downto 6) = "11")  THEN -- add/sub Rx, Ry; 
                    -- RXout, Ain
                    -- Check for XXX and based on the XXX -> select the register to output data to BUS
                    IF (IRoutf(5 downto 3) = "000") THEN Routf(0) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "001") THEN Routf(1) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "010") THEN Routf(2) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "011") THEN Routf(3) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "100") THEN Routf(4) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "101") THEN Routf(5) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "110") THEN Routf(6) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "111") THEN Routf(7) <= '1';
                    END IF;
                    Ainf <= '1'; -- select the register to import data to Ain
               END IF;
					
	     WHEN T2 =>
            IRinf <= '0';
            Routf <= "00000000";
            Goutf <= '0';
            DINoutf <= '0';
            Rinf <= "00000000";
            Ainf <= '0';
            AddSubf <= '0';
            Ginf <= '0';
            Done <= '0';
                IF (IRoutf(7 downto 6) = "10")  THEN -- add/sub Rx, Ry; 
                    -- RYout, Gin
                    -- Check for YYY and based on the YYY -> select the register to output data to BUS
                    IF (IRoutf(2 downto 0) = "000") THEN Routf(0) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "001") THEN Routf(1) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "010") THEN Routf(2) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "011") THEN Routf(3) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "100") THEN Routf(4) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "101") THEN Routf(5) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "110") THEN Routf(6) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "111") THEN Routf(7) <= '1';
                    END IF;
                    Ginf <= '1'; -- select the register to import data to Ain
                    AddSubf <= '0';
                ELSIF (IRoutf(7 downto 6) = "11")  THEN -- add/sub Rx, Ry; 
                    -- RYout, Gin AddSub
                    -- Check for YYY and based on the YYY -> select the register to output data to BUS
                    IF (IRoutf(2 downto 0) = "000") THEN Routf(0) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "001") THEN Routf(1) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "010") THEN Routf(2) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "011") THEN Routf(3) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "100") THEN Routf(4) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "101") THEN Routf(5) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "110") THEN Routf(6) <= '1';
                    ELSIF (IRoutf(2 downto 0) = "111") THEN Routf(7) <= '1';
                    END IF;
						  Ginf <= '1'; -- select the register to import data to Ain
                    AddSubf <= '1';						  
                END IF;

                   
        WHEN T3 =>	     
            IRinf <= '0';
            Ainf <= '0'; -- IF not the case Ainf will be changed  
            Routf <= "00000000";
            Goutf <= '0';
            DINoutf <= '0';
            Rinf <= "00000000";
            Ainf <= '0';
            AddSubf <= '0';
            Ginf <= '0';
            Done <= '0';					 
                -- Gout, RXin, Done
                -- Check for XXX and based on the XXX -> select the register to input data to register     
                IF (IRoutf(7 downto 6) = "10")  THEN -- add/sub Rx, Ry; 
					     Goutf <= '1';-- G register goes out to different registers                
                    IF (IRoutf(5 downto 3) = "000") THEN Rinf(0) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "001") THEN Rinf(1) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "010") THEN Rinf(2) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "011") THEN Rinf(3) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "100") THEN Rinf(4) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "101") THEN Rinf(5) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "110") THEN Rinf(6) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "111") THEN Rinf(7) <= '1';
                    END IF;
                ELSIF (IRoutf(7 downto 6) = "11")  THEN -- add/sub Rx, Ry; 
					     Goutf <= '1';-- G register goes out to different registers					 
                    IF (IRoutf(5 downto 3) = "000") THEN Rinf(0) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "001") THEN Rinf(1) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "010") THEN Rinf(2) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "011") THEN Rinf(3) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "100") THEN Rinf(4) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "101") THEN Rinf(5) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "110") THEN Rinf(6) <= '1';
                    ELSIF (IRoutf(5 downto 3) = "111") THEN Rinf(7) <= '1';
                    END IF;
                END IF;

                Done <= '1';
					 
          END CASE; 

      END PROCESS;
	 
end behavioral;
