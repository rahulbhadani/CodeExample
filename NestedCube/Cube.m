%% (C) Rahul Kumar Bhadani
% This program animates the generation of smaller cubes inside a bigger
% cube in an orderly fashion.
clf;
axis([-12 12 -12 12 -12 12]);
% Create a bigger cube of size 10
drawCube(-10,10,0,0,0)
alpha(0.1);
axis([-12 12 -12 12 -12 12]);

%Create smalled cubes of size 1
for i = 0:2:9
    for j = 0:2:9
        for k = 0:2:9
            drawCube(-10,2,i,j,k)
        end
    end
end