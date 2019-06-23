----------------------------------------------------------------------------------
-- Company: OFFIS e.V. 
-- Engineer: Markus Wieghaus
-- 
-- Create Date: 11/20/2014 01:35:25 PM
-- Design Name: 
-- Module Name: ppm_interpreter_logic - Behavioral
-- Project Name: 
-- Target Devices: Zynq 
-- Tool Versions: Vivado 2014.3
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ppm_interpreter_logic is
    generic (syncTime : integer := 250000;      -- realTime= syncTime* 1000ns => 2500 * 1000ns = 2500000ns = 2,5ms
             timeoutTime : integer := 210000    -- realTime= timeoutTime *1000ns => 2100000ns = 2,1ms => when this time is reached after transmission the transmission is done                           
            );
    Port ( clk : in STD_LOGIC;
           reset : in STD_LOGIC;
           
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
          
end ppm_interpreter_logic;

architecture Behavioral of ppm_interpreter_logic is

-- announce global signals and types
------------------------------------------------------

--component clkGenerator is 
--    generic (	rate :integer:=50000000; -- in Hz
--				clockFrequency : integer :=100000000); -- in Hz

--    Port ( clk : in  STD_LOGIC;
--           reset : in  STD_LOGIC;
--           genratedClkRate : out  STD_LOGIC);
--end component;








type stateType is (sync_state, first_low_state, channel_high_state, channel_low_state, timeOut);

type ram is array (0 to 31) of std_logic_vector (31 downto 0);

type stateMachine is record 
    state : stateType;
    -- timer variable
    timer : integer;
    
    -- channel index number
    index : integer;
    
    -- channel ram
    channel_ram: ram;
    
    -- registers for channels high and low times
    channel1_high  : std_logic_vector(31 downto 0);
    channel1_low   : std_logic_vector(31 downto 0);
    channel2_high  : std_logic_vector(31 downto 0);
    channel2_low   : std_logic_vector(31 downto 0);
    channel3_high  : std_logic_vector(31 downto 0);
    channel3_low   : std_logic_vector(31 downto 0);
    channel4_high  : std_logic_vector(31 downto 0);
    channel4_low   : std_logic_vector(31 downto 0);
    channel5_high  : std_logic_vector(31 downto 0);
    channel5_low   : std_logic_vector(31 downto 0);            
    channel6_high  : std_logic_vector(31 downto 0);
    channel6_low   : std_logic_vector(31 downto 0);            
    channel7_high  : std_logic_vector(31 downto 0);
    channel7_low   : std_logic_vector(31 downto 0);
    channel8_high  : std_logic_vector(31 downto 0);
    channel8_low   : std_logic_vector(31 downto 0);                                
    channel9_high  : std_logic_vector(31 downto 0);
    channel9_low   : std_logic_vector(31 downto 0);
    channel10_high : std_logic_vector(31 downto 0);
    channel10_low  : std_logic_vector(31 downto 0);
    channel11_high : std_logic_vector(31 downto 0);
    channel11_low  : std_logic_vector(31 downto 0);
    channel12_high : std_logic_vector(31 downto 0);
    channel12_low  : std_logic_vector(31 downto 0);
    channel13_high : std_logic_vector(31 downto 0);
    channel13_low  : std_logic_vector(31 downto 0);    
    channel14_high : std_logic_vector(31 downto 0);
    channel14_low  : std_logic_vector(31 downto 0);
    channel15_high : std_logic_vector(31 downto 0);
    channel15_low  : std_logic_vector(31 downto 0);
    channel16_high : std_logic_vector(31 downto 0);
    channel16_low  : std_logic_vector(31 downto 0);
                                                                                                                                                                    
 end record;
 
type input is record 
    ppm : std_logic;   
end record;    


signal state, nextState : stateMachine;
signal inputs : input;

constant minimumTime : integer := 48000;
--constant minimumLowTime  : integer := 160000
--signal newClk : std_logic;

-- end announce global signals and types
------------------------------------------------------

begin
-- init clkGenerator
------------------------------------------------------
--clkGen : clkgenerator

--port map(
--	clk =>clk,

--	reset => reset,

--	genratedClkRate => newClk
--);
-- end clkGenerator
------------------------------------------------------


-- signal mapping
------------------------------------------------------
    channel1_high  <= state.channel1_high;
    channel1_low   <= state.channel1_low;
    channel2_high  <= state.channel2_high;
    channel2_low   <= state.channel2_low;
    channel3_high  <= state.channel3_high;
    channel3_low   <= state.channel3_low;
    channel4_high  <= state.channel4_high;
    channel4_low   <= state.channel4_low;
    channel5_high  <= state.channel5_high;
    channel5_low   <= state.channel5_low;
    channel6_high  <= state.channel6_high;
    channel6_low   <= state.channel6_low;
    channel7_high  <= state.channel7_high;
    channel7_low   <= state.channel7_low;
    channel8_high  <= state.channel8_high;
    channel8_low   <= state.channel8_low;
    channel9_high  <= state.channel9_high;
    channel9_low   <= state.channel9_low;
    channel10_high <= state.channel10_high;
    channel10_low  <= state.channel10_low;
    channel11_high <= state.channel11_high;
    channel11_low  <= state.channel11_low;
    channel12_high <= state.channel12_high;
    channel12_low  <= state.channel12_low;
    channel13_high <= state.channel13_high;
    channel13_low  <= state.channel13_low;
    channel14_high <= state.channel14_high;
    channel14_low  <= state.channel14_low;
    channel15_high <= state.channel15_high;
    channel15_low  <= state.channel15_low;
    channel16_high <= state.channel16_high;
    channel16_low  <= state.channel16_low;
    


-- end signal mapping
------------------------------------------------------


-- begin processes
------------------------------------------------------


getInputs : process(clk)
    begin
        if rising_edge(clk) then
            inputs.ppm <= ppm;
        end if;
end process;
------------------------------------------------------


stateMachineClocking : process(clk, reset)
    begin
        if reset ='0' then 
            state.state <= sync_state;
            state.timer <=0;
            state.index <=0;
            for i in 0 to 31 loop 
                state.channel_ram(i)<=(others => '1');
            end loop;
             
            state.channel1_high  <= (others => '1');
            state.channel1_low   <= (others => '1');
            state.channel2_high  <= (others => '1');
            state.channel2_low   <= (others => '1');
            state.channel3_high  <= (others => '1');
            state.channel3_low   <= (others => '1');
            state.channel4_high  <= (others => '1');
            state.channel4_low   <= (others => '1');
            state.channel5_high  <= (others => '1');
            state.channel5_low   <= (others => '1');
            state.channel6_high  <= (others => '1');
            state.channel6_low   <= (others => '1');
            state.channel7_high  <= (others => '1');
            state.channel7_low   <= (others => '1');
            state.channel8_high  <= (others => '1');
            state.channel8_low   <= (others => '1');
            state.channel9_high  <= (others => '1');
            state.channel9_low   <= (others => '1');
            state.channel10_high <= (others => '1');
            state.channel10_low  <= (others => '1');
            state.channel11_high <= (others => '1');
            state.channel11_low  <= (others => '1');
            state.channel12_high <= (others => '1');
            state.channel12_low  <= (others => '1');
            state.channel13_high <= (others => '1');
            state.channel13_low  <= (others => '1');
            state.channel14_high <= (others => '1');
            state.channel14_low  <= (others => '1');
            state.channel15_high <= (others => '1');
            state.channel15_low  <= (others => '1');
            state.channel16_high <= (others => '1');
            state.channel16_low  <= (others => '1');
            
            
        elsif rising_edge(clk) then
            state <= nextState;
        end if;
end process;
------------------------------------------------------


machine : process(inputs, state)

    begin

        nextState<=state;

        case(state.state) is
    
            when sync_state =>
            
                nextState.index <= 0;
                for i in 0 to 31 loop 
                    nextState.channel_ram(i)<=(others => '0');
                end loop;
                
                if inputs.ppm = '1' then
                    nextState.timer <=state.timer+1;
                end if;
                
                if inputs.ppm = '0' and state.timer >= syncTime then        
                -- minimum waiting time for synchronisation: 2,5 ms 
                -- if then the signal is low then the first low state is reached        
                    nextState.timer<=1;  
                    nextState.state<=first_low_state;

                elsif inputs.ppm = '0' and state.timer < syncTime then
                    nextState.timer <= 0;
                end if;
                
            when first_low_state => 
            
                if inputs.ppm = '1' then
                    nextState.state<=channel_high_state;
                    nextState.timer<=1;
                end if;
                
            when channel_high_state =>
                if inputs.ppm ='1' then
                    nextState.timer <= state.timer +1;
                    if state.timer >= timeoutTime then  -- if 2,1ms are reached then timeout 
                        nextState.state<=timeOut;
                    end if;
                elsif inputs.ppm='0' and state.timer > minimumTime then
                    --nextState.channel_ram(state.index)<= std_logic_vector(to_unsigned(state.timer*1000,32));
                    nextState.channel_ram(state.index)<= std_logic_vector(to_unsigned(state.timer*10,32));
                    nextState.timer <= 1;
                    nextState.index <= state.index+1;
                    nextState.state <= channel_low_state;
                end if;
            
            when channel_low_state =>  
                if inputs.ppm ='0' then
                   nextState.timer <= state.timer+1;
                elsif inputs.ppm='1' then
                   --nextState.channel_ram(state.index)<= std_logic_vector(to_unsigned(state.timer*1000,32));
                   nextState.channel_ram(state.index)<= std_logic_vector(to_unsigned(state.timer*10,32));
                   nextState.timer <= 1;
                   if state.index < 31 then
                      nextState.index <= state.index+1;
                      nextState.state <= channel_high_state;
                   else
                      nextState.state <= timeOut;
                   end if; 
                end if;
                 
            when timeOut =>  
                
                -- set the new timing values with next clock step              
                nextState.channel1_high  <= state.channel_ram(0);
                nextState.channel1_low   <= state.channel_ram(1);
                nextState.channel2_high  <= state.channel_ram(2);
                nextState.channel2_low   <= state.channel_ram(3);
                nextState.channel3_high  <= state.channel_ram(4);
                nextState.channel3_low   <= state.channel_ram(5);
                nextState.channel4_high  <= state.channel_ram(6);
                nextState.channel4_low   <= state.channel_ram(7);
                nextState.channel5_high  <= state.channel_ram(8);
                nextState.channel5_low   <= state.channel_ram(9);
                nextState.channel6_high  <= state.channel_ram(10);
                nextState.channel6_low   <= state.channel_ram(11);
                nextState.channel7_high  <= state.channel_ram(12);
                nextState.channel7_low   <= state.channel_ram(13);
                nextState.channel8_high  <= state.channel_ram(14);
                nextState.channel8_low   <= state.channel_ram(15);
                nextState.channel9_high  <= state.channel_ram(16);
                nextState.channel9_low   <= state.channel_ram(17);
                nextState.channel10_high <= state.channel_ram(18);
                nextState.channel10_low  <= state.channel_ram(19);
                nextState.channel11_high <= state.channel_ram(20);
                nextState.channel11_low  <= state.channel_ram(21);
                nextState.channel12_high <= state.channel_ram(22);
                nextState.channel12_low  <= state.channel_ram(23);
                nextState.channel13_high <= state.channel_ram(24);
                nextState.channel13_low  <= state.channel_ram(25);
                nextState.channel14_high <= state.channel_ram(26);
                nextState.channel14_low  <= state.channel_ram(27);
                nextState.channel15_high <= state.channel_ram(28);
                nextState.channel15_low  <= state.channel_ram(29);
                nextState.channel16_high <= state.channel_ram(30);
                nextState.channel16_low  <= state.channel_ram(31);
                                 
                nextState.state <= sync_state;
                
            when others =>
                nextState.state <= sync_state;
        end case;
end process;

-- end begin processes
------------------------------------------------------


end Behavioral;
