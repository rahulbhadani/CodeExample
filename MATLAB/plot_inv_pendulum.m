% Inverted Pendulum Modeling
% By Rahul Bhadani
% Plot Inverted Pendulum

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
