function H = calcH(p1, p2)
%
% Calculates the 3 x 3 homography matrix based on matching features points
% in two images, called image1 and image2.
% Pre-conditions:
%     `p1` is an n x 2 matrix containing n feature points, where each row
%     holds the coordinates of a feature point in image1, and
%     `p2` is an n x 2 matrix where each row holds the coordinates of a
%     corresponding point in image2.  
% Post-conditions:
%     H is the 3 x 3 homography matrix, such that 
%     p1_homogeneous = H * [p2 ones(size(p2, 1), 1)]'
%     p1_homogeneous contains the transformed homogeneous coordinates of
%     p2 from image2 to image1.

n = size(p1, 1);
if n < 4
    error('Not enough points');
end
H = zeros(3, 3);  % Homography matrix to be returned

% 
% The following code computes an homography matrix, H, using all feature points
% in p1 and p2. Modify it to compute a homography matrix using the inliers
% of p2 and p2 as determined by RANSAC.
% The code solves for H by solving a linear system of equations of the form Ax=b
% where x is a 9 x 1 column vector that is being solved for and which defines
% the desired homography matrix, H.  One way of defining A and b is given in the
% lecture notes in the slides titled "Solving for Homography" where A is 2n x 9
% and b is a 2n x 1 vector of 0's.  Alternatively, but equivalently, in the code 
% below A and b are defined using each pair of corresponding points' homogeneous
% coordinates, and A is a 3n x 9 matrix and b is a 3n x 1 column vector.  

% RANSAC, 100 tries

bestH = zeros(3, 3);
best_count = 0;
best_points = zeros(n);

for ransac = 1 : 100
    
    r = randi([1 n], 1, 4); % Pick 4 random feature pairs

    % Use them to compute a homography
    A = zeros(4 * 3, 9);
    b = zeros(4 * 3, 1);
    
    for i = 1 : 4
        A(3 * (i-1) + 1, 1 : 3) = [p2(r(i), :), 1];
        A(3 * (i-1) + 2, 4 : 6) = [p2(r(i), :), 1];
        A(3 * (i-1) + 3, 7 : 9) = [p2(r(i), :), 1];
        b(3 * (i-1) + 1 : 3 * (i-1) + 3) = [p1(r(i), :), 1];
    end
    x = (A\b)';
    H = [x(1:3); x(4:6); x(7:9)];
    
    count = 0;
    good_points = zeros(n);
    
    % Check accuracy of homography with all other feature pairs
    for j = 1 : n
        p = ones(3, 1);
        p(1) = p2(j, 1);
        p(2) = p2(j, 2);
        q = H * p;
        q_cart = [(q(1) / q(3)), (q(2) / q(3))];
        D = vertcat(p1(j, :), q_cart);
        dist = pdist(D, 'euclidean');
        
        % Keep track of feature pairs that are consistent with homography
        if (dist < 3)
            count = count + 1;
            good_points(j) = 1;
        end
    end
    
    % Check to see if this new homography is even better than previous ones
    % Keep it as reference if so, as well as the set of points that make it
    if(count > best_count)
        best_count = count;
        best_points = good_points;
        bestH = H;
    end
end

% Gather best points found into contiguous list
best_points_list = zeros(best_count);

j = 1;
for i = 1 : n
    if (best_points(i) == 1)
        best_points_list(j) = i;
        j = j + 1;
    end
end

% Recompute final homography with our best set of consistent points
A = zeros(best_count * 3, 9);
b = zeros(best_count * 3, 1);
    
for i = 1 : best_count
    A(3 * (i-1) + 1, 1 : 3) = [p2(best_points_list(i), :), 1];
    A(3 * (i-1) + 2, 4 : 6) = [p2(best_points_list(i), :), 1];
    A(3 * (i-1) + 3, 7 : 9) = [p2(best_points_list(i), :), 1];
    b(3 * (i-1) + 1 : 3 * (i-1) + 3) = [p1(best_points_list(i), :), 1];
end

x = (A\b)';
H = [x(1:3); x(4:6); x(7:9)];
