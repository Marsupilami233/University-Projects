LIBRARY ieee;
USE ieee.std_logic_1164.all;

entity airplane is
port (
   clk : IN std_logic; -- rising edge triggered
	reset : IN std_logic; --active low
	ivalid : IN std_logic; -- ivalid --> if the input is valid
	smooth : IN std_logic; -- smooth -->  if the plane is stable
	alt_10k : IN std_logic; -- alt_10k if the plane is flying through 10k
	alt_25k : IN std_logic; -- alt_25k if the plane is flying through 25k
	seatbelt : OUT std_logic; -- passeger needs to wear seatbelt
	noelectronics : OUT std_logic -- no electronic is allowed
);

end airplane;
-- declare the new state
architecture Behavioral of airplane is
-- I am making the state machine 
type states is (below_10k, below_25k_above_10k, above_25k, smooth_count1, smooth_count2, smooth_count3, smooth_count4, smooth_count5);
-- the state changes in the process, we have to use a different process to handle that 
-- the state update only then the update reset and transition happens
signal current_state : states;
-- first the process do the transistion
-- second process update the output
BEGIN
-- THE FIRST PROCESS CHECK FOR THE STATES
   PROCESS (clk, reset, ivalid)
   BEGIN
	   IF (reset = '0') THEN
		   current_state  <= below_10k; --Upon reset, set the state to the initial state: on the ground and below 10k		
      ELSIF rising_edge(clk) and ivalid = '1' THEN --if there is a rising edge of clock, then power on the state machine
      -- The CASE statement checks the value of the state variable,
		CASE current_state IS
		
		   -- if the current state is 
			WHEN below_10k =>
			   IF alt_10k = '1' THEN
				   current_state  <= below_25k_above_10k ;-- plane come through 10k
				ELSIF alt_25k = '1' THEN               
				   current_state <= below_10k;       -- if something rare that happens the current state won't change
				END IF;
				
			WHEN below_25k_above_10k  =>
			   IF alt_10k = '1' THEN
				   current_state  <= below_10k; -- plane go down to 10k
				ELSIF alt_25k = '1' THEN
				   current_state  <= above_25k; -- plane go up to 25k				   
				END IF;				

			WHEN above_25k  =>
			   IF alt_10k = '1' THEN
				   current_state  <= above_25k; -- if something rare that happens the current state won't change
				ELSIF alt_25k = '1' THEN
				   current_state  <= below_25k_above_10k; -- plane go down to 25k			   
				ELSIF smooth = '1' THEN
				   current_state  <= smooth_count1; -- plane smooth->next state
			   END IF;    	

			WHEN smooth_count1 =>
			   IF alt_10k = '1' THEN
				   current_state  <= smooth_count1; -- if something rare that happens the current state won't change
				ELSIF alt_25k = '1' THEN
				   current_state  <= below_25k_above_10k ; -- plane go down to 25k			   
				ELSIF smooth = '1' THEN
				   current_state  <= smooth_count2 ; -- plane smooth->next state
			   ELSE
				   current_state <= above_25k; -- plane recount to above 25k
				END IF;    					
			
			WHEN smooth_count2 =>
			   IF alt_10k = '1' THEN
				   current_state  <= smooth_count2; -- if something rare that happens the current state won't change
				ELSIF alt_25k = '1' THEN
				   current_state  <= below_25k_above_10k ; -- plane go down to 25k			   
				ELSIF smooth = '1' THEN
				   current_state  <= smooth_count3 ;-- plane smooth->next state
			   ELSE
				   current_state <= above_25k; -- plane recount to above 25k
				END IF;    	
				
			WHEN smooth_count3 =>
			   IF alt_10k = '1' THEN
				   current_state  <= smooth_count3; -- if something rare that happens the current state won't change
				ELSIF alt_25k = '1' THEN
				   current_state  <= below_25k_above_10k ; -- plane go down to 25k			   
				ELSIF smooth = '1' THEN
				   current_state  <= smooth_count4 ;-- plane smooth->next state
			   ELSE
				   current_state <= above_25k; -- plane recount to above 25k
				END IF;
				
			WHEN smooth_count4 =>
			   IF alt_10k = '1' THEN
				   current_state  <= smooth_count4; -- if something rare that happens the current state won't change
				ELSIF alt_25k = '1' THEN
				   current_state  <= below_25k_above_10k ; -- plane go down to 25k			   
				ELSIF smooth = '1' THEN
				   current_state  <= smooth_count5 ;-- plane smooth->next state
			   ELSE
				   current_state  <= above_25k; -- plane recount to above 25k
				END IF;    		

			WHEN smooth_count5 =>
			   IF alt_10k = '1' THEN
				   current_state  <= smooth_count5; -- if something rare that happens the current state won't change
				ELSIF alt_25k = '1' THEN
				   current_state  <= below_25k_above_10k ; -- plane go down to 25k			   
				ELSIF smooth = '1' THEN
				   current_state  <= smooth_count5; -- plane smooth->next state
			   ELSE
				   current_state  <= above_25k; -- plane recount to above 25k					
				END IF;   
				
			WHEN others =>
			   current_state <= below_10k; -- if somthing rare that happens ,reset to the original state
		END CASE;
   END IF;		
	END PROCESS;

	-- output funtion  (THESE TWO STATEMENTS ARE CONCURRENT)
   noelectronics <= '1' when current_state = below_10k else '0';
	seatbelt <= '0' when current_state = smooth_count5 else '1';

end Behavioral;