%(C) RAHUL BHADANI
% MATLAB Example to solve a system of nonlinear equations using newton's
% method

syms x1 x2 x3

x_0 = [0.1, 0.1, -0.1];

dist = 200;

 F1 = f1(x1, x2, x3);
 F2 = f2(x1, x2, x3);
 F3 = f3(x1, x2, x3);

 
 F = [F1; F2; F3];
 J = [diff(F1, x1), diff(F1, x2), diff(F1, x3); ...
        diff(F2, x1), diff(F2, x2), diff(F2, x3);...
        diff(F3, x1), diff(F3, x2), diff(F3, x3)]; %Find Jacobian

   
while dist > 0.001 
    
    F_0 = double( subs(F, [x1, x2, x3], x_0) );
    J_0 = double( subs(J, [x1, x2, x3], x_0) );

    y_0 = -J_0\F_0;
    x_1 = x_0 + y_0';
    dist  = norm(x_1 - x_0);
    x_0 = x_1;
end

fprintf('[%f ,%f, %f] is the required solution.\n', x_0);


function ret1 = f1(x1, x2, x3)
    ret1 = 3.*x1 - cos(x2.*x3)-0.5;
end

function ret2 = f2(x1, x2, x3)
    ret2 = x1.^2  - 81.*(x2 + 0.1).^2 + sin(x3) + 1.06;
end

function ret3 = f3(x1, x2, x3)
    ret3 = exp(-x1.*x2) + 20.*x3 + (10.*pi - 3)./3;
end