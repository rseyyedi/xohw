----------------------------------------------------------------------------------
-- Company: OFFIS e.V. 
-- Engineer: Markus Wieghaus
-- 
-- Create Date:    10:08:45 03/03/2015 
-- Design Name:    CLK-Generator 
-- Module Name:    clkGenerator - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description:  This logic generates the input rate frequency 
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
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity clkGenerator is
	 generic (	rate :integer:=50000000;--1000000; -- in Hz
				clockFrequency : integer :=100000000); -- in Hz 
    Port ( clk : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           genratedClkRate : out  STD_LOGIC);
end clkGenerator;

architecture Behavioral of clkGenerator is

constant countLimit 		: std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned((clockFrequency/rate),32)srl 1); 
signal newClk : std_logic;
signal cnt : std_logic_vector(31 downto 0);
begin
	
	
	clkGenerator : process (clk,reset) 
	begin
		
		if reset = '0' then
			cnt  	<= (others =>'0');
			newClk  <= '0';
		elsif rising_edge(clk) then	
			cnt <= std_logic_vector(unsigned(cnt) + 1);
			if cnt = countLimit then 
				cnt <= (others =>'0');		
				newClk <= NOT newClk;
			end if;
		end if;
	end process;

genratedClkRate <= newClk;

end Behavioral;

