library IEEE;
use IEEE.STD_LOGIC_1164.all, IEEE.NUMERIC_BIT.all;

library UNISIM;
use UNISIM.all; 

entity RAM is

    port(
        CLK : in BIT; -- синхровход
        R : in BIT; -- сброс
        WR: in BIT; -- сигнал записи
        AE: in BIT; -- сигнал фиксации адреса
        OE: in BIT; -- сигнал выдачи прочитанного слова
        AD: in STD_LOGIC_VECTOR(15 downto 0); -- адрес/данное
        DO: out STD_LOGIC_VECTOR(15 downto 0) -- прочитанное слово
    );
    
end RAM;


architecture BEH of RAM is

    type MEM1KX16 is array(0 to 4095) of BIT_VECTOR(15 downto 0);
    
    constant RAM_init: MEM1KX16:= --начальное состояние памяти
        (X"0000",X"0000",X"0000",X"0000",X"0000",X"0000", others=>X"0000");
    
    signal addr, dobuf: BIT_VECTOR(15 downto 0);
    --signal addri:natural;
    
begin
    RG_ADDR:process(CLK,R) begin --регистр адреса
        
        if R='1' then
            addr<=X"0000";
        elsif CLK='1' and CLK'event and AE='1' then
            addr<= To_BITvector(AD);
        end if;
        
    end process;
    
    ------ собственно блок памяти ------------------
    RAM1K:process(CLK,addr)

        variable RAM: MEM1KX16:= RAM_init;
        variable addri:natural;

    begin
        addri := To_INTEGER(unSIGNED(addr(15 downto 0)));
        if CLK='1' and CLK'event then
            if WR = '1' then
                RAM(addri):= To_BITvector(AD); -- запись
            end if;
        
            if R='1' then
                dobuf<= X"0000";
            else 
                dobuf<= RAM(addri);
            end if;
        end if;
        
    end process;
    
    TRI:DO <= To_StdLogicVector(dobuf)
        when OE='1' -- тристабильный выходной буфер
        else "ZZZZZZZZZZZZZZZZ";

end BEH; 