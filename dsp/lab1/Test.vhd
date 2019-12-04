library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.numeric_std.all;

entity Test is
end Test;

architecture t of Test is

-- inputs
signal CLK, RST : std_logic;

-- intermediate
signal genDataOut : real;
signal noise : real;
signal sum : real;

-- outputs
signal ADDR, DISTRIB : integer;
signal AVG_OUT : real;
signal AVG_NN : integer;

begin
    gen_unit : entity work.Gen 
        port map (CLK => CLK, RST => RST, DATA_OUT => genDataOut);
    
    gauss_unit : entity work.GaussGen
        port map (CLK => CLK, RST => RST, DATA_OUT => noise);
    
    add2_unit : entity work.Add2
        port map (A => genDataOut, B => noise, S => sum);
    
    avg_unit : entity work.Averager
        port map (CLK => CLK, RST => RST, DATA => sum, DATA_OUT => AVG_OUT, NN => AVG_NN);
        
    distr_unit : entity work.S_Distribut
        port map (CLK => CLK, RST => RST, ADDR => ADDR, DISTRIB => DISTRIB);

    -- continuous clock
    process 
    begin
        CLK <= '0';
        wait for 20 ns;
        CLK <= '1';
        wait for 20 ns;
    end process;

    RST <= '1', '0' after 20 ns;
end t;