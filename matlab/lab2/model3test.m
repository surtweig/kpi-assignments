[x,y] = ode45(@f3, [0 1], [1; 1]);

plot(x,y(:,1),'-o',x,y(:,2),'-o')
title('model3');
xlabel('x');
ylabel('y;z');
legend('y','z')