function [ img ] = feather_mask( I )
% Makes feathered alpha channel for given input image. Center pixels have
% max opacity with linear falloff as distance from center pixels increases.

img = rgb2gray(I);
img = im2bw(img, 0.0000001); % Threshold for finding image
img = imcomplement(img); % Flip 0s with 1s so bwdist feathers inwards
sides = edge(img, 'sobel'); % Find where the image is
border = strel('rectangle', [1 0]);
sides = imdilate(sides,border);
alpha = img;
alpha(sides > 0) = 0;

img = bwdist(alpha, 'euclidean');
img = mat2gray(img);

end

