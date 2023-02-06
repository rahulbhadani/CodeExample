% Inverted Pendulum Modeling
% By Rahul Bhadani
% Reference material: https://ctms.engin.umich.edu/CTMS/?example=InvertedPendulum&section=SystemModeling
%% Parameters
M = 0.5; % mass of the cart
m = 0.2; %mass of the pendulum
b = 0.1; % coefficient of friction
I = 0.06;
g = 9.8;
l = 0.3;
q = (M+m)*(I+m*l^2)-(m*l)^2;


%% s variable for laplace transfer
s = tf('s');

%% Transfer function for Pendulum
P_pend = (m*l*s/q)/(s^3 + (b*(I + m*l^2))*s^2/q - ((M + m)*m*g*l)*s/q - b*m*g*l/q);


%% PID
Kp = 100;
Ki = 1;
Kd = 1;
C = pid(Kp,Ki,Kd);

%% Feedback
T = feedback(P_pend,C);

%% Simulated Response
t = 0:0.05:5;
r =3*sin(t);
%r = ones(size(t));

% H1=c2d(T,0.05);
theta=lsim(T,r,t);

P_cart = (((I+m*l^2)/q)*s^2 - (m*g*l/q))/(s^4 + (b*(I + m*l^2))*s^3/q - ((M + m)*m*g*l)*s^2/q - b*m*g*l*s/q);
T2 = feedback(1,P_pend*C)*P_cart;

% H2=c2d(T2,0.05);
pos = lsim(T2, r,t);



%% Plotting
close all;
f = figure;
f.Position = [995 62 1448 1260];
%%
for index = 1:length(t)
    subplot(2,2,1);
    plot(t(index),theta(index), 'bo' );
    title('\theta')
    xlim([0 5]);
%     ylim([0 11]);
    hold on;
    subplot(2,2,2);
    plot(t(index),r(index), 'go');
    title('Amplitude of the Sine Wave (Input Applied Force)')
    hold on;
    drawnow;
    xlim([0 5]);
    ylim([-3 3]);
%     ylim([0 1]);
    subplot(2,2,[3,4]);
    plot_inv_pendulum(theta(index), pos(index), M, m, b, l, I);
    xlim([-2 2]);
    hold off;
    %pause(0.1);
end



%% Plotting with no control
t = 0:0.05:15;
r =0*sin(t);
r = r + normrnd(0,1e-12, size(r));
theta0 = lsim(P_pend, r, t);
theta0 = wrapToPi(theta0);
pos0=lsim(P_cart,r,t);

f = figure;
f.Position = [995 62 1448 1260];
for index = 1:length(t)
    subplot(2,1,1);
    plot(t(index),theta0(index), 'bo' );
    title('\theta')
    xlim([0 15]);
%     ylim([0 11]);
    hold on;
    subplot(2,1,2);
    plot_inv_pendulum(theta0(index), pos0(index), M, m, b, l, I);
    xlim([-10 10]);
    ylim([-2 10]);
    title(sprintf('t = %f ', t(index) ));
    hold off;
    pause(0.05);
    
end



function plot_inv_pendulum(theta, x, M, m, b, l, I)
    % Plot the cart
    x_corners = [x-0.5, x+0.5, x+0.5, x-0.5, x-0.5];
    y_corners = [-2, -2, 0, 0, -2];
    plot(x_corners, y_corners, 'k', 'LineWidth', 3);
    hold on;
    
    % Plot the pendulum
    x1 = x - 5*l * sin(theta);
    y1 = 5*l * cos(theta);
    plot([x, x1], [0, y1], 'k', 'LineWidth', 3);
    
    % Set plot limits
    xlim([x-2, x+2]);
    ylim([-2, 2]);
    
    % Add title and labels
    title('Inverted Pendulum on a Cart');
    xlabel('Position (m)');
    ylabel('Height (m)');
    
    % Turn on grid
    grid on;
end




