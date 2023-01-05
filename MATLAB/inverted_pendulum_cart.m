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

%% Developing Controller
K = 10;
C = K*(s+1)^2/s;
T = feedback(P_pend,C);

%% Impulse Response
t = 0:0.01:10;
impulse(T,t), grid
title({'Response of Pendulum Position to an Impulse Disturbance';'under Closed-loop Control'});


%% Controller 2
K = 35;
C = K*(s+1)*(s+2)/s;
T = feedback(P_pend,C);
t = 0:0.01:10;
impulse(T, t), grid
title({'Response of Pendulum Position to an Impulse Disturbance';'under Closed-loop Control'});


%%  Impulse Response of the car position
P_cart = (((I+m*l^2)/q)*s^2 - (m*g*l/q))/(s^4 + (b*(I + m*l^2))*s^3/q - ((M + m)*m*g*l)*s^2/q - b*m*g*l*s/q);
T2 = feedback(1,P_pend*C)*P_cart;
T2 = minreal(T2);
t = 0:0.01:10;
impulse(T2, t), grid
title({'Response of Cart Position to an Impulse Disturbance';'under Closed-loop Control'});

%% Simulated Response

r = t.^2 + t + 3;  
[y,t,x]=lsim(T2,r,t);

plot(t,y,'plot');
set(get(AX(1),'Ylabel'),'String','cart position (m)');


%% Statspace modeling
M = 0.5;
m = 0.2;
b = 0.1;
I = 0.006;
g = 9.8;
l = 0.3;

p = I*(M+m)+M*m*l^2; %denominator for the A and B matrices

A = [0      1              0           0;
     0 -(I+m*l^2)*b/p  (m^2*g*l^2)/p   0;
     0      0              0           1;
     0 -(m*l*b)/p       m*g*l*(M+m)/p  0];
B = [     0;
     (I+m*l^2)/p;
          0;
        m*l/p];
C = [1 0 0 0;
     0 0 1 0];
D = [0;
     0];

states = {'x' 'x_dot' 'phi' 'phi_dot'};
inputs = {'u'};
outputs = {'x'; 'phi'};

sys_ss = ss(A,B,C,D,'statename',states,'inputname',inputs,'outputname',outputs);

%% Controllability
co = ctrb(sys_ss);
controllability = rank(co);

%% 
close all;
Q = C'*C;
R = 1;
K = lqr(A,B,Q,R)

Ac = [(A-B*K)];
Bc = [B];
Cc = [C];
Dc = [D];

states = {'x' 'x_dot' 'phi' 'phi_dot'};
inputs = {'r'};
outputs = {'x'; 'phi'};

sys_cl = ss(Ac,Bc,Cc,Dc,'statename',states,'inputname',inputs,'outputname',outputs);

t = 0:0.05:500;
r =3*sin(t);
[y,t,x]=lsim(sys_cl,r,t);
cart_position = y(:,1);
pendulum_angle = y(:,2);
f = figure;
f.Position = [995          62        1448        1260];

for index = 1:length(t)
    subplot(3,2,1);
    plot(t(index),cart_position(index) , 'ro');
    xlim([0 150]);
%     ylim([0 1]);
    hold on;
    subplot(3,2,3);
    plot(t(index),pendulum_angle(index), 'bo' );
    xlim([0 150]);
%     ylim([0 11]);
    hold on;
    subplot(3,2,5);
    plot(t(index),r(index) , 'go');
    hold on;
    drawnow;
    xlim([0 150]);
%     ylim([0 1]);
    subplot(3,2,[2,4]);
    plot_inv_pendulum(pendulum_angle(index), cart_position(index), M, m, b, l, I);
    xlim([-4 4]);
    hold off;
    %pause(0.0001);
end