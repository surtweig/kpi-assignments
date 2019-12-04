library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.math_real.all;

entity Gen is

generic
(
    n1:natural:= 100;
    n2:natural:= 200
);

port
(
    CLK : in STD_LOGIC;
    RST : in STD_LOGIC;
    DATA_OUT : out REAL:= 0.0;
    START : out BIT
);
end Gen;

architecture MODEL of Gen is
    
signal ct2:natural;

begin
    process(CLK,RST)
    begin
        if RST='1' then
            ct2<=0;
            START<='1';
        elsif CLK='1' and CLK'event then 
            START<='0';
        
            if ct2=n1+n2-1 then
                ct2<=0;
            else
                ct2<=ct2+1;
            end if;
        
            if ct2<=n1 and ct2>0 then
                DATA_OUT<= arctan(real(ct2)/real(n1));
            else
                DATA_OUT<= cos(MATH_PI*real(ct2-n1)/real(n2));
            end if;
        end if;
    end process;
end MODEL;
