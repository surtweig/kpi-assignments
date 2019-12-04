   library IEEE;
   use IEEE.STD_LOGIC_1164.all;
   entity S_Distribut is 
   generic(channels:natural:=100;
      		drange:real:=2.0;
     		dmin:real:=-1.0);
      	port(	CLK, RST: in STD_LOGIC;		
      		DATA : in REAL:=0.0;
      		ADDR : out INTEGER;
      		DISTRIB : out INTEGER);
   end S_Distribut;
   architecture MODEL of S_Distribut is  
      	type Tarr is array (0 to channels+1) of natural;	
      	signal Distr_arr:Tarr:=(others=>0);
      	signal addri,aa:natural;
      	signal even: STD_LOGIC;  
   begin
     process(CLK)  
        variable addrw:integer;	
     begin
     addrw:=integer(real(channels)*(DATA - dmin)/drange - 0.5)+1;
          if 	DATA> (dmin+drange) then
		addrw:=channels+1;
          elsif 	DATA<=dmin	then
		addrw:=0;
          end if;	
      			
      		aa<=addrw;
      		if RST='1' then
      			Distr_arr<=(others=>0);
      		elsif CLK='1' and CLK'event then
      			Distr_arr(addrw)<=Distr_arr(addrw)+1;
      		end if;
      	end process;

      	process(CLK)
      	begin
      		if RST='1' then
      			addri<=0;
      			even<='0';
      		elsif CLK='1' and CLK'event then
      			if addri=channels+1 then
      				addri<=0;
      				even<=not even;
      			else
      				addri<=addri+1;
      			end if;
      		end if;
      	end process;

      	ADDR<=addri when even='1' else -1;
      	DISTRIB<=Distr_arr(ADDRi) when even='1' else -1;
end MODEL;