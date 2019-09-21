% (C )
figure;
subplot(1,3,1);
plot(front_right_q);
hold on;
plot(front_left_q);
title('Position');
legend({'Front Right Position','Front Left Position'});

subplot(1,3,2);
plot(front_right_w);
hold on;
plot(front_left_w);
title('Velocity');
legend({'Front Right Position','Front Left Position'});

subplot(1,3,3);
plot(front_right_t);
hold on;
plot(front_left_t);
title('Torque');
legend({'Front Right Position','Front Left Position'});

figure;

plot(front_right_q.data, front_right_w.data);