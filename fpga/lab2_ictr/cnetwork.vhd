library IEEE;
use IEEE.std_logic_1164.all;
--use IEEE.std_logic_arith.all;
use IEEE.numeric_std.all;

package CNetwork is

    --преобразование вектора бит - числа в доп. коде в целое
    function BIT_TO_INT(b:BIT_VECTOR) return integer;
    
    --преобразование бита в целое
    function BIT_TO_INT(b:BIT) return integer;

    --преобразование целого в вектор бит - число в доп. коде
    function INT_TO_BIT(i,l:integer) return BIT_VECTOR;

    --преобразование целого 0|1 в бит
    function INT_TO_BIT(i:integer) return BIT;
   
    function MaxInt(a,b:integer) return integer;

end package CNetwork;

package body CNetwork is

    function BIT_TO_INT(b:BIT_VECTOR) return integer is
    begin
        return to_integer(signed(to_stdlogicvector(b)));
        --return conv_integer(signed(to_stdlogicvector(b)));
    end;

    function BIT_TO_INT(b:BIT) return integer is
    begin
        if b='1' then
            return 1;
        else
            return 0;
        end if;
    end;

    function INT_TO_BIT(i,l:integer) return BIT_VECTOR is
    begin
        --return to_bitvector(conv_std_logic_vector(i, l));
        --return to_bitvector(conv_std_logic_vector(i, l));        
        return to_bitvector(std_logic_vector(to_signed(i, l)));
    end;

    function INT_TO_BIT(i:integer) return BIT is
    begin
        if i=1 then
            return '1';
        else
            return '0';
        end if;
    end;
    
    function MaxInt(a,b:integer) return integer is
    begin
        if a>b then
            return a;
        else
            return b;
        end if;
    end;

end package body CNetwork;