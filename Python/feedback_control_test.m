s = tf('s');
sys = tf(4, [1 2 10], 'ts', 0.000001);
figure;
impulse(sys);

figure;
step(sys);

%%
H = tf([-0.06,0.4],[1,-1.6,0.78],0.01);
tFinal = 5;
step(H, tFinal);
figure;
impulse(H, tFinal);

%% s variable for laplace transfer

M = 0.5;
m = 0.2;
b = 0.1;
I = 0.06;
g = 9.8;
l = 0.3;
q = (M+m)*(I+m*l^2)-(m*l)^2;

s = tf('s');
% Transfer function for Pendulum
P_pend = (m*l*s/q)/(s^3 + (b*(I + m*l^2))*s^2/q - ((M + m)*m*g*l)*s/q - b*m*g*l/q);

Kp = 100;
Ki = 1;
Kd = 1;
C = pid(Kp,Ki,Kd);

T = feedback(P_pend,C);

t = 0:0.05:50;
figure;
r =3*sin(t);
lsim(T,r,t);

