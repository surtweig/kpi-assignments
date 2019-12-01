function dydt = f3(x, y)
    dydt = [(y(2)-y(1))*x; (y(2)+y(1))*x];