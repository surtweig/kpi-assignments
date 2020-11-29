library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

library work;
use work.cnetwork.all;

entity conversion_test is

    port(
        bit4 : in BIT_VECTOR(3 downto 0);
        bit1 : in BIT;
        int4 : integer;
        int1 : integer;
        
        converted_int4 : out integer;
        converted_int1 : out integer;
        converted_bit4 : out BIT_VECTOR(3 downto 0);
        converted_bit1 : out BIT
    );

end conversion_test;


architecture Behavior of conversion_test is

begin
    converted_int4 <= BIT_TO_INT(bit4);
    converted_int1 <= BIT_TO_INT(bit1);
    converted_bit4 <= INT_TO_BIT(int4, 4);
    converted_bit1 <= INT_TO_BIT(int1);
end Behavior;