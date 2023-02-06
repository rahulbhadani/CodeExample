x = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]; % input sequence
X = ztransform(x); % compute Z-transform
magnitude = abs(X); % compute magnitude of Z-transform

figure;
stem(magnitude); % plot the magnitude of the Z-transform
hold on;
stem(x);
legend('X', 'x')
title('Magnitude of Z-transform');
xlabel('k');
ylabel('|X(k)|');

X'

% P = 100;
% I = 1;
% D = 1;
% T = 0.05;
% 
% Tz = (( P + ((I*T)/2) + (D/T) )*(z.^2) +  ( -P + ((I*T)/2) - ((2*D)/T) )*z  + (D/T) )/ ( (z.^2) - z);
% 
% X = Tz./x;
% 
function X = ztransform(x)
    N = length(x);
%     z = exp(-1i*2*pi/N);
    z = exp(1i*2*pi/N)
%     z = 1;
    X = zeros(1, N);
    for k = 0:N-1
        z
        z.^(k*(0:N-1))'
        X(k+1) = sum(x.*z.^(k*(0:N-1)));
    end
end

