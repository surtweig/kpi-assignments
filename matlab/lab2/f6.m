function dydt = f6(x, y)
    dydt = [y(2); -y(2)/x + y(1)/(x*x) + 1];