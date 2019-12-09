
global sigma
global rho
global beta
sigma = 10;
rho = 28;
beta = 8/3;
%linkdata on
fig = figure;
sigma_tb = uicontrol('Parent',fig,'Style','slider','Position',[81,54,419,23],...
                     'value',sigma, 'min',0, 'max',30);
sigma_text = uicontrol('Parent',fig,'Style','text','Position',[240,25,100,23],...
                     'String','Sigma');

rho_tb = uicontrol('Parent',fig,'Style','slider','Position',[81,54+60,419,23],...
                     'value',rho, 'min',0, 'max',30);
rho_text = uicontrol('Parent',fig,'Style','text','Position',[240,25+60,100,23],...
                     'String','Rho');

beta_tb = uicontrol('Parent',fig,'Style','slider','Position',[81,54+120,419,23],...
                     'value',beta, 'min',0, 'max',10);
beta_text = uicontrol('Parent',fig,'Style','text','Position',[240,25+120,100,23],...
                     'String','Beta');
          
                 
sigma_tb.Callback = @(es,ed) updateLorenz('sigma', es.Value);%updateSystem(p,tf(wn^2,[1,2*(es.Value)*wn,wn^2])); 
rho_tb.Callback = @(es,ed) updateLorenz('rho', es.Value);%updateSystem(p,tf(wn^2,[1,2*(es.Value)*wn,wn^2])); 
beta_tb.Callback = @(es,ed) updateLorenz('beta', es.Value);%updateSystem(p,tf(wn^2,[1,2*(es.Value)*wn,wn^2])); 

function p = updateLorenz(valname, val)
global sigma;
global rho;
global beta;
if strcmp(valname, 'sigma')
    sigma = val;
elseif strcmp(valname, 'rho')
    rho = val;
elseif strcmp(valname, 'beta')
    beta = val;
end
f = @(t,a) [-sigma*a(1) + sigma*a(2); rho*a(1) - a(2) - a(1)*a(3); -beta*a(3) + a(1)*a(2)];
[t,a] = ode45(f,[0 100],[1 1 1]);     % Runge-Kutta 4th/5th order ODE solver
p = plot3(a(:,1),a(:,2),a(:,3));
end