library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity Add2 is

generic
(
    ma:real:=1.0;
    mb:real:=1.0
);

port
(
    A, B: in REAL;
    S : out REAL:=0.0
);

end Add2;

architecture Add2 of Add2 is
begin
    S <= ma*A + mb*B;
end Add2;