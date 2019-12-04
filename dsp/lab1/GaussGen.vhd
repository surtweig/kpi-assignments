library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.math_real.all;

entity GaussGen is

generic
(
  nn:natural:=1; --power of the binomial distribution <16
  m:REAL:=0.0    -- mean output value
);

port
(
    CLK : in STD_LOGIC;
    RST : in STD_LOGIC;
    DATA_OUT : out REAL:= 0.0
);

end GaussGen;

architecture Model of GaussGen is

type arri is array (0 to 15) of integer;
type arrr is array (0 to 15) of real;

begin

    SFR : process(clk,rst)
        variable s1:arri:=(3,33,333,3,4,5,6,7,8,9,11,22,33,others=>55);
        variable s2:arri:=(5,55,555,50,6,7,8,9,5,6,7,21,33,others=>22);
        variable r:arrr:=(others=>0.0);
        variable s:real:=0.0;
    begin
        if rst='1' then
            DATA_OUT<=0.0;
        elsif  clk='1' and clk'event then
            s:=0.0;
            for i in 0 to nn-1 loop    -- nn noise generators
                UNIFORM (s1(i),s2(i),r(i));
                s:=s+r(i);
            end loop;
            DATA_OUT <= 2.0*(s/real(nn)-0.5)+ m;
        end if;
    end process;
    
end Model;