quitbutton=uicontrol('style','pushbutton', ...
   'string','Quit', ...
   'fontsize',12, ...
   'position',[0,0,50,20], ...
   'callback','stop=1; close;');

width = 100;
height = 100;
cells = (rand(width, height)) > 0.5;
img = image(cat(3, cells, cells, cells));
stop = 0;

x = 1:width;
y = 1:height;
xr = mod(x+1-1, width)+1;
xl = mod(x-1-1, width)+1;
yu = mod(y+1-1, height)+1;
yd = mod(y-1-1, height)+1;
sum = zeros(width, height);

while (stop == 0)
    sum(x, y) = cells(x, yu) + cells(xr, yu) + cells(xr, y) + cells(xr, yd) + ...
                cells(x, yd) + cells(xl, yd) + cells(xl, y) + cells(xl, yu);
    
    cells = (sum == 3) | (sum == 2 & cells);
            
    set(img, 'cdata', cat(3, cells, cells, cells))
    drawnow
    pause(0.1)
end