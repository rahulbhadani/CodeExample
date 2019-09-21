%% M_6_2.m
% SWT 10-12-16
% using ray-transfer matrices to calculate
% focal length of a spherical lens
% Housekeeping
clear; 


nglass = 1.3; nair = 1;
diaLens = 4; rocLens = diaLens/2;
P1 = (nair - nglass)/(-rocLens);
P2 = (nglass - nair)/(rocLens);

L_n = diaLens/nglass;
test = 10; f = 0;

while abs(test) > 0.0005
    f = f + 0.0001;
    TM1 = [ 1 0; f 1]; TM2 = [1 0 ; L_n 1];
    RM1 = [ 1 -P1; 0 1]; RM2 = [1 -P2; 0 1];
    
    Mlens = RM1*TM2*RM2;
    M = TM1*Mlens;
    
    test = M(2, 2);
end
Mlens
M