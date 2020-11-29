library ieee;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_1164.all;

library work;
use work.cnetwork.all;

entity lab1_lsm is

    generic(
        Width:integer:= 12
    );

    

    port(
        F : in BIT;                           -- функция
        A : in BIT_VECTOR(Width-1 downto 0);  -- первый операнд
        B : in BIT_VECTOR(Width-1 downto 0);  -- второй операнд
        C0: in BIT;                           -- вход переноса
        Y : out BIT_VECTOR(Width-1 downto 0); -- результат
        C3: out BIT;                          -- выход переноса
        Z : out BIT;                          -- признак нулевого результата
        N : out BIT                           -- знак результата
    ); 
    
end lab1_lsm; 


architecture BEH of lab1_lsm is

    signal ai,bi,ci,yi:INTEGER;
    signal ybi:BIT_VECTOR(Width downto 0);
    constant zeros : bit_vector(Width-1 downto 0) := (others => '0');

begin
    -- представляем входные данные целыми -----------------
    ai<= BIT_TO_INT(A);
    bi<= BIT_TO_INT(B);
    ci<= BIT_TO_INT(C0);
 
    -- Сумматор ------------------------------------
    ADDER:
        yi<= ai+bi+ci;

    -- Мультиплексор результата ----------------------------------
    MUX:
        with F select ybi<=
            INT_TO_BIT(yi,Width+1) when '0',--сумматор
            '0'&INT_TO_BIT(MaxInt(ai, bi), Width) when others; 

        C3<= ybi(Width); -- выходной перенос
        Z<= '1' when ybi(Width-1 downto 0) = zeros else '0';--призн.нуля
        N<= ybi(Width-1);
        Y<= ybi(Width-1 downto 0); -- Результат
end BEH;