% (C) RAHUL BHADANI
% A Simple example to demonstrate Minkowski Sum by Hand.

Ax = [0 0 2 2];
Ay = [0 1 1 0];

Bx = [0 2];
By = [0 3];

%Minkowski Sum
Mx = [0 0 2 4 4 2];
My = [0 1 4 4 3 0];
figure(1);
subplot(2,2,1);
F = fill(Ax, Ay, 'cyan');
F.FaceColor = [0.25, 0.74, 0.95];
F.FaceAlpha = 0.8;
F.EdgeColor = 'none';
xlim([-1 5]);
ylim([-1 5]);
set(gca,'FontSize',15);
set(gca,'FontName','Times');
title('\textbf{A}','Interpreter','latex');
grid on;
grid minor;
axis equal;

subplot(2,2, 2);
L = line(Bx, By);
L.Color = [0.74, 0.25, 0.95];
xlim([-1 5]);
ylim([-1 5]);
set(gca,'FontSize',15);
set(gca,'FontName','Times');
title('\textbf{B}','Interpreter','latex');
grid on;
grid minor;
axis equal;

subplot(2,2,3:4);
F = fill(Mx, My, 'cyan');
F.FaceColor = [0.95, 0.24, 0.75];
F.FaceAlpha = 0.8;
F.EdgeColor = 'none';
xlim([-1 5]);
ylim([-1 5]);
set(gca,'FontSize',15);
set(gca,'FontName','Times');
title('\textbf{Minkowski Sum}','Interpreter','latex');
grid on;
grid minor;
axis equal;


