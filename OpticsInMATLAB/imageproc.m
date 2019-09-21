% (C) RAHUL BHADANI

%% An example to read image file and swap R G B color channels

% Read the image in the same folder
IMG = imread('actuator.jpg');

% Separate color planes
red = IMG(:,:,1);
green = IMG(:,:,2);
blue = IMG(:,:,3);



% Swap Colors
IMG_SWAP(:,:,1) = green;
IMG_SWAP(:,:,2) = blue;
IMG_SWAP(:,:,3) = red;
figure(1);
subplot(2,1,1);
image(IMG);
subplot(2,1,2);
imagesc(IMG_SWAP);
