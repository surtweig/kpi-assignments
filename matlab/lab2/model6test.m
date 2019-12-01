[x,y] = ode45(@f6, [3 13], [6; 3]);

plot(x,y(:,1),'-o',x,y(:,2),'-o')
title('model3');
xlabel('x');
ylabel('y;z');
legend('y','z')