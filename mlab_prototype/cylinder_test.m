img_in = imread('cyltest.jpg');
img_in = im2double(img_in);

f = 270;

y_size = size(img_in, 1);
x_size = size(img_in, 2);
y_center = y_size / 2;
x_center = x_size / 2;

cyl_img = zeros(y_size, x_size, 3);

% For each pixel in an unwrapped cylinder...
for y_cyl = 1 : y_size
    for x_cyl = 1 : x_size
        
        % ...using inverse cylindrical coordinates...
        theta = (x_cyl - x_center) / f;
        h = (y_cyl - y_center) / f;
        x_hat = sin(theta);
        y_hat = h;
        z_hat = cos(theta);
        
        % ...find corresponding pixel in input image...
        x = round(f * x_hat / z_hat + x_center);
        y = round(f * y_hat / z_hat + y_center);
   
        % ...and translate to unwrapped cylindrical map if pixel exists.
        if (x > 0 && x <= x_size && y > 0 && y <= y_size)
            cyl_img(y_cyl, x_cyl, :) = img_in(y, x, :);
        end
    end
end

imshow(cyl_img);