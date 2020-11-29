library IEEE;
use IEEE.Numeric_BIT.all;

library work;
use work.CNetwork.all;


entity ICTR is
    
    port(
        CLK : in BIT; -- синхровход
        R : in BIT; -- сброс
        WR: in BIT; -- сигнал записи
        D : in BIT_VECTOR(12 downto 0); -- адрес перехода
        F : in BIT_VECTOR(2 downto 0); -- функция
        A : out BIT_VECTOR(12 downto 0)
    ); -- выходной адрес
    
end ICTR; 


architecture BEH of ICTR is

    signal RG: BIT_VECTOR(2 downto 0); -- регистр мл.части адреса
    signal SM: unSIGNED(3 downto 0); -- сумматор мл.части адреса
    signal CTR:unSIGNED(12 downto 3);-- счетчик ст.части адреса
    
begin
    -- описание сумматора ------------------------------------
    SUM:SM<= Resize(unSIGNED(RG),4)+unSIGNED(F);
    -- описание RG -------------------------------------------
    
    R_3:process(R,CLK)
    begin
        if R='1' then
            RG<="000";
        elsif CLK='1' and CLK'event then
            case F is
                when "001"|"010"|"011"|"100"=>RG<=BIT_VECTOR(SM(2 downto 0));
                when "101" => RG<="000";
                when "110" => RG<=D(2 downto 0);
                when others=> null;
            end case;
        end if;
    end process;
    
    CT:process(CLK,R) -- описание счетчика ---------------------------------------
    begin
        if R='1' then
            CTR<="0000000000";
        elsif CLK='1' and CLK'event then
            if F="101" then
                CTR<="0000000000";
            elsif F="110" then
                CTR<= unSIGNED(D(12 downto 3));
            elsif (F(2) = '0' or F="100") and SM(3) ='1' then
                CTR<= unSIGNED(CTR)+1;
            end if;
        end if;
    end process;
    
    A<=BIT_VECTOR(CTR)&RG; -- выходной адрес
    
end BEH; 