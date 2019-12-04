library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity Averager is 

generic (N:integer:=300); -- Averaging period
      
port
(
    CLK, RST: in STD_LOGIC;
    DATA : in real;
    DATA_OUT : out real;     
    NN:out integer
);

end Averager;


architecture beh of Averager is

type Tarr is array(0 to N-1) of real;
signal arr : Tarr:=(others=>0.0);      
signal addr:natural; 
signal NNi:natural:=1;

begin
    process(CLK,RST)
    begin
        if RST='1' then
            arr<= (others=>0.0); 
            addr<=0;
            NNi<=1;
        elsif CLK='1' and CLK'event then
            arr(addr)<= arr(addr)+DATA; 
            if addr = N-1 then
                addr<=0;
                NNi<=NNi+1;
            else
                addr<=(addr+1) ;
            end if;
        end if;
    end process;

    DATA_OUT<=arr(addr)/real(NNi);
    NN<=NNi;   
end beh;