----------------------------------------------------------------------------------
-- Company:  OFFIS e.V. 
-- Engineer: Markus Wieghaus
-- 
-- Create Date: 11/25/2014 10:57:17 AM
-- Design Name: Testbench for ppm logic
-- Module Name: testbench_ppm - Behavioral
-- Project Name: 
-- Target Devices: Zynq 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity testbench_ppm is
--  Port ( );
end testbench_ppm;

architecture Behavioral of testbench_ppm is
   -- Component Declaration for the Unit Under Test (UUT)

 COMPONENT ppm_interpreter_logic
generic (syncTime : integer := 250000;      
          timeoutTime : integer := 210000                              
        );
 PORT(
      clk : IN  std_logic;
      reset : IN  std_logic;
       -- ppm input signal
      ppm : in STD_LOGIC;
                
      -- outputs
      -- 16 channels with high and low time  
     channel1_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel1_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel2_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel2_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel3_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel3_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel4_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel4_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel5_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel5_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel6_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel6_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel7_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel7_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel8_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel8_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel9_high     : out STD_LOGIC_VECTOR (31 downto 0);
     channel9_low      : out STD_LOGIC_VECTOR (31 downto 0);
     channel10_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel10_low     : out STD_LOGIC_VECTOR (31 downto 0);
     channel11_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel11_low     : out STD_LOGIC_VECTOR (31 downto 0);
     channel12_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel12_low     : out STD_LOGIC_VECTOR (31 downto 0);
     channel13_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel13_low     : out STD_LOGIC_VECTOR (31 downto 0);
     channel14_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel14_low     : out STD_LOGIC_VECTOR (31 downto 0);
     channel15_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel15_low     : out STD_LOGIC_VECTOR (31 downto 0);
     channel16_high    : out STD_LOGIC_VECTOR (31 downto 0);
     channel16_low     : out STD_LOGIC_VECTOR (31 downto 0)
     );
 END COMPONENT;
 

--Inputs
signal clk : std_logic := '0';
signal reset : std_logic := '0';
signal ppm : std_logic := '1';


  --Outputs
 signal channel1_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel1_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel2_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel2_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel3_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel3_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel4_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel4_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel5_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel5_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel6_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel6_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel7_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel7_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel8_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel8_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel9_high     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel9_low      :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel10_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel10_low     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel11_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel11_low     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel12_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel12_low     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel13_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel13_low     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel14_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel14_low     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel15_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel15_low     :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel16_high    :  STD_LOGIC_VECTOR (31 downto 0);
 signal channel16_low     :  STD_LOGIC_VECTOR (31 downto 0);

-- Clock period definitions
constant clk_period : time := 10 ns;
begin
	-- Instantiate the Unit Under Test (UUT)
uut: ppm_interpreter_logic 
generic map (
      syncTime => 250000,
      timeoutTime => 210000
)
PORT MAP (
      clk => clk,
      reset => reset,
      ppm=> ppm,
      channel1_high=>channel1_high,
      channel1_low => channel1_low,
      channel2_high=>channel2_high,
      channel2_low => channel2_low,
      channel3_high=>channel3_high,
      channel3_low => channel3_low,
      channel4_high=>channel4_high,
      channel4_low => channel4_low,
      channel5_high=>channel5_high,
      channel5_low => channel5_low,
      channel6_high=>channel6_high,
      channel6_low => channel6_low,
      channel7_high=>channel7_high,
      channel7_low => channel7_low,
      channel8_high=>channel8_high,
      channel8_low => channel8_low,
      channel9_high=>channel9_high,
      channel9_low => channel9_low,
      channel10_high=>channel10_high,
      channel10_low => channel10_low,
      channel11_high=>channel11_high,
      channel11_low => channel11_low,
      channel12_high=>channel12_high,
      channel12_low => channel12_low,
      channel13_high=>channel13_high,
      channel13_low => channel13_low,
      channel14_high=>channel14_high,
      channel14_low => channel14_low,
      channel15_high=>channel15_high,
      channel15_low => channel15_low,
      channel16_high=>channel16_high,
      channel16_low => channel16_low
    );

-- Clock process definitions
clk_process :process
begin
    clk <= '0';
    wait for clk_period/2;
    clk <= '1';
    wait for clk_period/2;
end process;


-- Stimulus process
stim_proc: process
begin        
  -- hold reset state for 100 ns.
    reset<='0';
  wait for 100 ns;    
    reset<='1';
    
    
    
  wait for  5ms;
  -- first low state
   ppm<= '0';
   wait for 0.5ms;
   
   -- channel 1 
   ppm <= '1';
   wait for 1ms;
   
   ppm <= '0'; 
   wait for 0.5ms;
   
   -- channel 2
   ppm<='1';
   wait for 1.2ms;
   
   ppm <='0';
   wait for 0.5ms;
   -----------
   -- timeout
   ppm <='1';
   
   wait for  5ms;
    ---------
    -- first low state
   ppm<= '0';
   wait for 0.5ms;
       
   -- channel 1 
   ppm <= '1';
   wait for 1.5ms;
       
   ppm <= '0'; 
   wait for 0.5ms;
       
   -- channel 2
   ppm<='1';
   wait for 1.3ms;
       
   ppm <='0';
   wait for 0.5ms;

   -- channel 3
   ppm<='1';
   wait for 1.125ms;
         
   ppm <='0';
   wait for 0.5ms;
     
   -- timeout
   ppm <='1';
   ----- 

    
    wait for clk_period*10;

    
    
  wait;
end process;


end Behavioral;
