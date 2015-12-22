%%======================================================================
%% Script to create a panorama image from a sequence of images in `imagepath' directory.
%
% Note:  This skeleton code does NOT include all steps or all details
% required.
%
% Hint: This script is divided into several code sections delimited by the
% double percentage sign '%%'. Click 'Editor > Run Section' in MATLAB to
% run your current code section.
% 
%%======================================================================
%% 1. Take images
% 
% Change the following to the folder containing your input images
clear;
%imagepath = 'test';
%addpath '.\test';
imagepath = 'input_images';
addpath '.\input_images';
% Assumes images are in order and are the *only*
% files in this directory.
% 
%                         
%%======================================================================
%% 1.5 Cylindrical Projection

% Map each input image into cylindrical projection before computing
% homographies/translation

files = dir(imagepath);
initlist = files(3:end);
mkdir('cylindrical_maps');
%addpath '.\cylindrical_maps';

% Choose a focal length that looks right for particular input
f = 400;

for i=1:length(initlist)
    
    img = imread(initlist(i).name);
    img = im2double(img);    
    y_size = size(img, 1);
    x_size = size(img, 2);
    y_center = y_size / 2;
    x_center = x_size / 2;

    cyl_map = zeros(y_size, x_size, 3);

    % For each pixel in an unwrapped cylinder...
    for y_cyl = 1 : y_size
        for x_cyl = 1 : x_size
        
            % ...using inverse cylindrical coordinates...
            theta = (x_cyl - x_center) / f;
            h = (y_cyl - y_center) / f;
            x_hat = sin(theta);
            y_hat = h;
            z_hat = cos(theta);
        
            % ...find corresponding pixels in input image...
            x = round(f * x_hat / z_hat + x_center);
            y = round(f * y_hat / z_hat + y_center);
   
            % ...to populate the unwrapped cylindrical map.
            if (x > 0 && x <= x_size && y > 0 && y <= y_size)
                cyl_map(y_cyl, x_cyl, :) = img(y, x, :);
            end
        end
    end
    
    directory = 'cylindrical_maps/';
    img_index = int2str(i);
    extension = '.jpg';
    filename = strcat(directory, img_index, extension);
    imwrite(cyl_map, filename);

end

% Use newly created cylindrical projections for feature detection
imagepath = 'cylindrical_maps';

%%======================================================================
%% 2. Compute feature points, 3. Compute homographies
%
% Modify the calcH function to add code for implementing RANSAC.
% You do not have to modify this code section.
%
% Here we compute feature points and the homography matrices between
% adjacent input images. Homography matrices between adjacent input images
% are then stored in a cell array H_list.

% Read in the list of filenames of images to be processed
files = dir(imagepath);

% Eliminate . and .. and assume everything else in directory is an input
% image. Also assume that all images are color.
imagelist = files(3:end);

for i=1:length(imagelist)-1
    image1 = fullfile(imagepath, imagelist(i).name);
    image2 = fullfile(imagepath, imagelist(i+1).name);
      
    % Find matching feature points between current two images using SIFT
    % features and SIFT algorithm for matching. Note that this code does
    % NOT include the use of RANSAC to find and use only good matches.
    [~, matchIndex, loc1, loc2] = match(image1, image2);
    im1_ftr_pts = loc1(find(matchIndex > 0), 1:2);
    im2_ftr_pts = loc2(matchIndex(find(matchIndex > 0)), 1:2);

    % Calculate 3x3 homography matrix, H, mapping coordinates in image2
    % into coordinates in image1. Function calcH currently uses all pairs
    % of matching feature points returned by the SIFT algorithm.

    % Modify the calcH function to add code for implementing RANSAC.
    H = calcH(im1_ftr_pts, im2_ftr_pts);
    %H = c(im1_ftr_pts, im2_ftr_pts);
    H_list(i) = {H};
end

%%======================================================================
%% 4. Warp images / Translate cylindrical projections
%
% Select one input image as the reference image, ideally one in the middle
% of the sequence of input images so that there is less distortion in the
% output.
%
% Compute new homographies H_map that map every other image *directly* to
% the reference image by composing H matrices in H_list. Save these new
% homographies in a list called H_map. Hence H_map is a list of 3 x 3
% homography matrices that map each image into the reference image's
% coordinate system.
% 
% The homography in H_map that is associated with the reference image
% should be the identity matrix, created using eye(3) The homographies in
% H_map for the other images (both before and after the reference image)
% are computed by using already defined matrices in H_map and H_list as
% described in the homework.
%
% Note: Composing A with B is not the same as composing B with A.
% Note: H_map and H_list are cell arrays, which are general containers in
% Matlab. For more info on using cell arrays, see:
% http://www.mathworks.com/help/matlab/matlab_prog/what-is-a-cell-array.html

%------------- YOUR CODE STARTS HERE -----------------
% 
% Compute new homographies H_map that map every other image *directly* to
% the reference image 
%
% To create wide panorama, project input images on cylindrical coordinates
% and use homographies found in 2 & 3 to determine angular translation
% between images.

% Choose reference image
%ref_index = floor(length(imagelist) / 2) + 1;

% First image as reference
H_map(1) = {eye(3)}; %ID

H_21 = H_list{1};
H_32 = H_list{2};
H_43 = H_list{3};

H_map(2) = {H_21};
H_31 = H_21 * H_32;
H_map(3) = {H_31};
H_41 = H_21 * H_32 * H_43;
H_map(4) = {H_41};

%------------- YOUR CODE ENDS HERE -----------------

% Compute the size of the output panorama image
min_row = 1;
min_col = 1;
max_row = 0;
max_col = 0;

% for each input image
for i=1:length(H_map)
    cur_image = imread(fullfile(imagepath, imagelist(i).name));
    [rows,cols,~] = size(cur_image);
    
    % create a matrix with the coordinates of the four corners of the
    % current image
    pt_matrix = cat(3, [1,1,1]', [1,cols,1]', [rows, 1,1]', [rows,cols,1]');
    
    % Map each of the 4 corner's coordinates into the coordinate system of
    % the reference image
    for j=1:4
        result = H_map{i}*pt_matrix(:,:,j);
    
        min_row = floor(min(min_row, result(1)));
        min_col = floor(min(min_col, result(2)));
        max_row = ceil(max(max_row, result(1)));
        max_col = ceil(max(max_col, result(2))); 
    end
    
end

% Calculate output image size
panorama_height = max_row - min_row + 1;
panorama_width = max_col - min_col + 1;

% Calculate offset of the upper-left corner of the reference image relative
% to the upper-left corner of the output image
row_offset = 1 - min_row;
col_offset = 1 - min_col;

% Perform inverse mapping for each input image
for i=1:length(H_map)
    
    cur_image = im2double(imread(strcat(imagepath,'/',imagelist(i).name)));
    
    % Create a list of all pixels' coordinates in output image
    [x,y] = meshgrid(1:panorama_width, 1:panorama_height);
    % Create list of all row coordinates and column coordinates in separate
    % vectors, x and y, including offset
    x = reshape(x,1,[]) - col_offset;
    y = reshape(y,1,[]) - row_offset;
    
    % Create homogeneous coordinates for each pixel in output image
    pan_pts(1,:) = y;
    pan_pts(2,:) = x;
    pan_pts(3,:) = ones(1,size(pan_pts,2));
    
    % Perform inverse warp to compute coordinates in current input image
    image_coords = H_map{i}\pan_pts;
    row_coords = reshape(image_coords(1,:),panorama_height, panorama_width);
    col_coords = reshape(image_coords(2,:),panorama_height, panorama_width);
    % Note:  Some values will return as NaN ("not a number") because they
    % map to points outside the domain of the input image
    
    % Bilinear interpolate color values
    curr_warped_image = zeros(panorama_height, panorama_width, 3);
    for channel = 1 : 3
        curr_warped_image(:, :, channel) = interp2(cur_image(:,:,channel), ...
            col_coords, row_coords, 'linear', 0);
    end
    
    % Add to output image. No blending done in this version; the current
    % image simply overwrites previous images where there is overlap.
    warped_images{i} = curr_warped_image;
        
end


%%======================================================================
%% 5. Blend images
%
% Now that we've warped each input image separately and assigned them to
% warped_images (a cell array with as many elements as the number of input
% images), blend the input images into a single panorama.

% Initialize output image to black (0)
panorama_image = zeros(panorama_height, panorama_width, 3);

%------------- YOUR CODE STARTS HERE -----------------
%
% Modify the code below to blend warped images together via feathering The
% following code adds warped images directly to panorama image. This is a
% very bad blending method - implement feathering instead.

% Place first image in panorama
panorama_image = panorama_image + warped_images{1};

% At each boundary of input images...
for i = 2 : length(warped_images)
    % Draw newer image one on top of the existing panorama
    panorama_image = panorama_image + warped_images{i};
    
    % With both images...
    img1 = warped_images{i - 1};
    img2 = warped_images{i};
    
    % Apply feathering mask...
    alpha1 = feather_mask(img1);
    alpha2 = feather_mask(img2);

    r1 = alpha1 .* img1(:, :, 1);
    g1 = alpha1 .* img1(:, :, 2);
    b1 = alpha1 .* img1(:, :, 3);
    
    r2 = alpha2 .* img2(:, :, 1);
    g2 = alpha2 .* img2(:, :, 2);
    b2 = alpha2 .* img2(:, :, 3);
    
    mask1 = zeros(size(img1));
    mask2 = zeros(size(img2));
    
    mask1(:, :, 1) = r1;
    mask1(:, :, 2) = g1;
    mask1(:, :, 3) = b1;
    
    mask2(:, :, 1) = r2;
    mask2(:, :, 2) = g2;
    mask2(:, :, 3) = b2;
    
    % In the region of overlap, blend
    overlap = zeros(panorama_height, panorama_width);
    for x = 1 : panorama_width
        for y = 1 : panorama_height
            if(img1(y, x) > 0 && img2(y, x) > 0)
                panorama_image(y, x, :) = (mask1(y, x, :) + mask2(y, x, :)) / (alpha1(y, x) + alpha2(y, x));
            end
        end
    end        
end

%imshow(panorama_image);
imwrite(panorama_image, 'ftsao.jpg');

%rmdir('cylindrical_maps', 's');

% Save your final output image as a .jpg file and name it according to
% the directions in the assignment.  
%
%------------- YOUR CODE ENDS HERE -----------------
