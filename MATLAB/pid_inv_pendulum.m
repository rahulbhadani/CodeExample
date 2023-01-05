% Inverted Pendulum Modeling
% By Rahul Bhadani
% Reference material: https://ctms.engin.umich.edu/CTMS/?example=InvertedPendulum&section=SystemModeling
%% Parameters
M = 0.5;
m = 0.2;
b = 0.1;
I = 0.006;
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
t = 0:6.e-02:5;
r =3*sin(t);
%r = ones(size(t));

theta=lsim(T,r,t);

P_cart = (((I+m*l^2)/q)*s^2 - (m*g*l/q))/(s^4 + (b*(I + m*l^2))*s^3/q - ((M + m)*m*g*l)*s^2/q - b*m*g*l*s/q);
T2 = feedback(1,P_pend*C)*P_cart;

pos = lsim(T2,r,t);



%% Plotting
close all;
f = figure;
f.Position = [995          62        1448        1260];

%%
for index = 1:length(t)
    subplot(2,2,1);
    plot(t(index),theta(index), 'bo' );
    xlim([0 1]);
%     ylim([0 11]);
    hold on;
    subplot(2,2,2);
    plot(t(index),r, 'go');
    hold on;
    drawnow;
    xlim([0 1]);
%     ylim([0 1]);
    subplot(2,2,[3,4]);
    plot_inv_pendulum(theta(index), pos(index), M, m, b, l, I);
    xlim([-2 2]);
    hold off;
    %pause(0.1);
end

