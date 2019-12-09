library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity LUTDisplay is

port (
     XValue : in std_logic_vector(7 downto 0);
     HEXH : out std_logic_vector(6 downto 0);
     HEXL : out std_logic_vector(6 downto 0)
);

end LUTDisplay;

architecture main of LUTDisplay is
begin
     process (XValue)
          variable wL : std_logic_vector(3 downto 0):= X"0";
          variable wH : std_logic_vector(3 downto 0):= X"0";        
     begin
          wL(3 downto 0):= XValue(3 downto 0);
          wH(3 downto 0):= XValue(7 downto 4);
          
--    --0--
--    5   1
--    |-6-|
--    4   2
--    --3--
          
          case wH is
               when X"0" => HEXH <= b"1000000";
               when X"1" => HEXH <= b"1111001";
               when X"2" => HEXH <= b"0100100";
               when X"3" => HEXH <= b"0110000";
               when X"4" => HEXH <= b"0011001";
               when X"5" => HEXH <= b"0010010";
               when X"6" => HEXH <= b"0000010";
               when X"7" => HEXH <= b"1111000";
               when X"8" => HEXH <= b"0000000";
               when X"9" => HEXH <= b"0010000";
               when X"A" => HEXH <= b"0001000"; -- A
               when X"B" => HEXH <= b"0000011"; -- b
               when X"C" => HEXH <= b"1000110"; -- C
               when X"D" => HEXH <= b"0100001"; -- d
               when X"E" => HEXH <= b"0000110"; -- E
               when X"F" => HEXH <= b"0001110"; -- F
               when others => HEXH <= b"1111111";
          end case;
          
          case wL is
               when X"0" => HEXL <= b"1000000";
               when X"1" => HEXL <= b"1111001";
               when X"2" => HEXL <= b"0100100";
               when X"3" => HEXL <= b"0110000";
               when X"4" => HEXL <= b"0011001";
               when X"5" => HEXL <= b"0010010";
               when X"6" => HEXL <= b"0000010";
               when X"7" => HEXL <= b"1111000";
               when X"8" => HEXL <= b"0000000";
               when X"9" => HEXL <= b"0010000";
               when X"A" => HEXL <= b"0001000"; -- A
               when X"B" => HEXL <= b"0000011"; -- b
               when X"C" => HEXL <= b"1000110"; -- C
               when X"D" => HEXL <= b"0100001"; -- d
               when X"E" => HEXL <= b"0000110"; -- E
               when X"F" => HEXL <= b"0001110"; -- F
               when others => HEXL <= b"1111111";
          end case;  
     end process;
end main;