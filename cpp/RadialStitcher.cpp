////////////////////////////////////////////////////////////////////////////////
//
// Program: Radial Stitcher
// Author: Felix Tsao
// Contact: hello@felixtsao.com
//
// Summary:
//
//     Simple panorama image/video stitcher. Assumes input frames are taken
//     from radially symmetric viewpoints i.e. after projection, stitching
//     challenge is reduced to recovering angles between cameras
//     i.e. translation in the video plane space.
//
//     Developed using OpenCV 3.1.0 release and Ubuntu 16.04
//
//     Written using feature descriptors from OpenCV. This code is free to be
//     copied and modified and adheres to the same OpenCV license below.
//
//
////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this
//  license. If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote
//     products derived from this software without specific prior written
//     permission.
//
// This software is provided by the copyright holders and contributors "as is"
// and any express or implied warranties, including, but not limited to, the
// implied warranties of merchantability and fitness for a particular purpose
// are disclaimed. In no event shall the Intel Corporation or contributors be
// liable for any direct, indirect, incidental, special, exemplary, or
// consequential damages (including, but not limited to, procurement of
// substitute goods or services; loss of use, data, or profits; or business
// interruption) however caused and on any theory of liability, whether in
// contract, strict liability, or tort (including negligence or otherwise)
// arising in any way out of the use of this software, even if advised of the
// possibility of such damage.
//
////////////////////////////////////////////////////////////////////////////////

// Standard
#include <iostream>
#include <vector>
#include <cmath>

// OpenCV 3.1.0
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

// Radial Stitcher
#include "RadialStitcher.hpp"
#include "VectorSpace.hpp"

////////////////////////////////////////////////////////////////////////////////


// Initialize stitcher parameters and prewarp images
RadialStitcher::RadialStitcher(int numImages, char ** fileNames){

    // Initialize stitcher parameters...
    this->numImages = numImages;
    projection = SPHERICAL;
    focalLength = 2800; // LA Skyline (300mm)
    focalLength = 2800; // SF Golden Gate (Moto G4 Camera)

    // ...and warp and store input images
    for (int i = 0; i < numImages; i++) {

        cv::Mat temp = cv::imread(fileNames[i + 1], 1);
        cv::Mat mask = cv::Mat::zeros(temp.rows, temp.cols, CV_64F);
        buildBlendMask(temp, mask);

        if(!temp.data){ // Image is legit?
            std::cout << "Images could not be read." << std::endl;
            exit(1);
        }

        src.push_back(cv::Mat::zeros(temp.rows, temp.cols, temp.type()));
        blendMasks.push_back(cv::Mat::zeros(mask.rows, mask.cols, mask.type()));

        // Project image and its blend mask
        if(projection == SPHERICAL){
            RadialStitcher::projectSpherical(temp, src[i], focalLength);
            RadialStitcher::projectMaskSpherical(mask, blendMasks[i], focalLength);
        } else {
            projectCylindrical(temp, src[i], focalLength);
            projectCylindrical(mask, blendMasks[i], focalLength);
        }

    }

}


RadialStitcher::~RadialStitcher(){}


// Projects an image onto a cylindrical surface with specified focal length
// -----------------------------------------------------------------------------
int RadialStitcher::projectCylindrical(cv::Mat &I, cv::Mat&O, double f){

    int nRows = I.rows;
    int nCols = I.cols;

    int xCenter = nCols / 2;
    int yCenter = nRows / 2;

    int x, y;
    uchar *sI;
    uchar *sO;

    // For every pixel in cylindrical projection
    for (y = 0; y < nRows; y++) {
        for (x = 0; x < nCols; x++) {

            double theta = (x - xCenter) / f;
            double h = (y - yCenter) / f;
            double xp = sin(theta);
            double yp = h;
            double zp = cos(theta);

            // Get pixel from input image
            int xIn = round(f * xp / zp + xCenter);
            int yIn = round(f * yp / zp + yCenter);

            // In bounds?
            sO = O.ptr<uchar>(y);
            if(xIn > -1 && xIn < nCols && yIn > -1 && yIn < nRows){
                sI = I.ptr<uchar>(yIn);
                // 3 Channel BGR
                sO[3*x] = sI[3*xIn]; // Blue
                sO[3*x + 1] = sI[3*xIn + 1]; // Green
                sO[3*x + 2] = sI[3*xIn + 2]; // Red
            }
        }
    }

    return 0;
}


// Projects an image onto a spherical surface, returns warped image
// -----------------------------------------------------------------------------
int RadialStitcher::projectSpherical(cv::Mat &I, cv::Mat&O, double f){

    int nRows = I.rows;
    int nCols = I.cols;

    int xCenter = nCols / 2;
    int yCenter = nRows / 2;

    int x, y;
    uchar *sI;
    uchar *sO;

    // Inverse spherical projection
    // For every pixel in spherical projection
    // TODO can probably optimize this
    for (y = 0; y < nRows; y++) {
        for (x = 0; x < nCols; x++) {

            double theta = (x - xCenter) / f;
            double phi = (y - yCenter) / f;
            double xp = sin(theta) * cos(phi);
            double yp = sin(phi);
            double zp = cos(theta) * cos(phi);

            // Get pixel from input image
            int xIn = round(f * xp / zp + xCenter);
            int yIn = round(f * yp / zp + yCenter);

            // In bounds?
            sO = O.ptr<uchar>(y);
            if(xIn > -1 && xIn < nCols && yIn > -1 && yIn < nRows){
                sI = I.ptr<uchar>(yIn);
                // 3 Channel RGB
                sO[3*x] = sI[3*xIn]; // Blue
                sO[3*x + 1] = sI[3*xIn + 1]; // Green
                sO[3*x + 2] = sI[3*xIn + 2]; // Red
            }
        }
    }

    return 0;

}


// Projects a mask onto a spherical surface, returns warped mask
// -----------------------------------------------------------------------------
int RadialStitcher::projectMaskSpherical(cv::Mat &I, cv::Mat&O, double f){

    int nRows = I.rows;
    int nCols = I.cols;

    int xCenter = nCols / 2;
    int yCenter = nRows / 2;

    int x, y;
    double *sI;
    double *sO;

    // Inverse spherical projection
    // For every pixel in spherical projection
    for (y = 0; y < nRows; y++) {
        for (x = 0; x < nCols; x++) {

            double theta = (x - xCenter) / f;
            double phi = (y - yCenter) / f;
            double xp = sin(theta) * cos(phi);
            double yp = sin(phi);
            double zp = cos(theta) * cos(phi);

            // Get pixel from input image
            int xIn = round(f * xp / zp + xCenter);
            int yIn = round(f * yp / zp + yCenter);

            // In bounds?
            sO = O.ptr<double>(y);
            if(xIn > -1 && xIn < nCols && yIn > -1 && yIn < nRows){
                sI = I.ptr<double>(yIn);
                sO[x] = sI[xIn];
            }
        }
    }

    return 0;

}


// Makes an alpha/feathering mask for a given image
// Pixels in center of image have alpha value 1 and linearly decreases to 0
// towards edges of image
// -----------------------------------------------------------------------------
int RadialStitcher::buildBlendMask(cv::Mat& img, cv::Mat& mask){

    if(img.size() != mask.size()) return -1; // Check same size

    int nRows = mask.rows;
    int nCols = mask.cols;
    int maxDist = 0;

    if(nRows > nCols) maxDist = nCols / 2;
    else maxDist = nRows / 2;

    mask = cv::Scalar(0, 0, 0); // No alpha yet

    double* row;

    int distLeft, distRight, distTop, distBottom; // Distances from edge of image

    // Compute alpha intensity for each pixel in mask
    for (int y = 0; y < nRows; y++) {
        row = mask.ptr<double>(y);
        distTop = abs(y + 1);
        distBottom = abs(nRows - y - 1);
        int yDist = std::min(distTop, distBottom);
        for (int x = 0; x < nCols; x++) {
            distLeft = abs(x + 1);
            distRight = abs(nCols - x - 1);
            int xDist = std::min(distLeft, distRight);
            row[x] = (double) std::min(xDist, yDist) / (double) maxDist; // alpha
        }
    }

    return 0;

}


// Get feature points for two adjacent images and determines matching pairs
// Uses ORB descriptor under the hood from OpenCV
// -----------------------------------------------------------------------------
int RadialStitcher::getFeatures(cv::Mat& img1, cv::Mat& img2){

    // Feature Descriptor Parameters///////////////
    int numFeatures = 2000;
    float scaleFactor = 2.0;
    int numLevels = 8;
    int edgeThreshold = 31;
    int firstLevel = 0;
    int WTA_K = 2;
    int scoreType = cv::ORB::HARRIS_SCORE;
    int patchSize = 31;
    int fastThreshold = 20;
    ///////////////////////////////////////////////

    cv::Ptr<cv::ORB> detector = cv::ORB::create(
        numFeatures,
        scaleFactor,
        numLevels,
        edgeThreshold,
        firstLevel,
        WTA_K,
        scoreType,
        patchSize,
        fastThreshold);

    // Clear previous feature information from different image pair
    keypoints1.clear();
    keypoints2.clear();
    matches.clear();
    cv::Mat descriptors1; // curr's
    cv::Mat descriptors2; // neighbor's

    detector->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);


    // Determine matching pairs via Euclidean distance in feature space
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<cv::DMatch> approxMatches;
    matcher.match(descriptors1, descriptors2, approxMatches);

    // Find minimum keypoint error
    double minDist = 1000;
    for(int i = 0; i < descriptors1.rows; i++){
        double dist = approxMatches[i].distance;
        if(dist < minDist) minDist = dist;
    }

    // Keep only "good" matches i.e. those that are less than 3 * minDist
    for(int i = 0; i < descriptors1.rows; i++){
        if(approxMatches[i].distance < 6 * minDist) matches.push_back(approxMatches[i]);
    }

    /*
    cv::Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2,
        matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    //imwrite("match.jpg", img_matches);
    cv::imshow("Matches", img_matches);
    cv::waitKey(0);
    */

    return 0;

}


// Compute translation homography between 2 adjacent images
// -----------------------------------------------------------------------------
int RadialStitcher::estimateHomography(cv::Mat& homography){

    int nMatches = (int) matches.size();

    std::cout << nMatches << " feature point matches" << std::endl;

    int trials = 4000;
    double tolerance = 4.0; // pixels
    int concensus = 0;
    int maxConcensus = 0;
    Matrix bestHomography(3,3);

    srand(0);

    // RANSAC

    if(nMatches < 4) std::cout << "Not enough feature pairs" << std::endl;

    // Estimate quality is high at around 500 iterations
    for (int i = 0; i < trials; i++) {

        // Inititialize
        concensus = 0;

        //int featureNumber = rand() % nMatches; // Choose random feature pair
        int featureNumber = 1; // Choose random feature pair
        //std::cout << "ftNumb " << featureNumber << std::endl;

        // Solve Ah = b for h
        // h contains homography between image pair

        // Feature pair coordinates between images A and B
        double ax[4];
        double bx[4];
        double ay[4];
        double by[4];

        // Choose 4 feature pairs at random, sample without replacement
        int ft[4];
        for (int j = 0; j < 4; j++) { ft[j] = rand() % nMatches; }


        // Store for convenience to build matrix A to find homography
        for (int k = 0; k < 4; k++) {
            ax[k] = keypoints1[matches[ft[k]].queryIdx].pt.x;
            bx[k] = keypoints2[matches[ft[k]].trainIdx].pt.x;
            ay[k] = keypoints1[matches[ft[k]].queryIdx].pt.y;
            by[k] = keypoints2[matches[ft[k]].trainIdx].pt.y;
        }


        Matrix A(8,8); // Fill in A to solve Ah = b using LU decomposition

        A(0,0) = bx[0]; A(0,1) = by[0]; A(0,2) = 1; A(0,3) = 0; A(0,4) = 0; A(0,5) = 0; A(0,6) = -bx[0] * ax[0]; A(0,7) = -by[0] * ax[0];

        A(1,0) = bx[1]; A(1,1) = by[1]; A(1,2) = 1; A(1,3) = 0; A(1,4) = 0; A(1,5) = 0; A(1,6) = -bx[1] * ax[1]; A(1,7) = -by[1] * ax[1];

        A(2,0) = bx[2]; A(2,1) = by[2]; A(2,2) = 1; A(2,3) = 0; A(2,4) = 0; A(2,5) = 0; A(2,6) = -bx[2] * ax[2]; A(2,7) = -by[2] * ax[2];

        A(3,0) = 0; A(3,1) = 0; A(3,2) = 0; A(3,3) = bx[0]; A(3,4) = by[0]; A(3,5) = 1; A(3,6) = -bx[0] * ay[0]; A(3,7) = -by[0] * ay[0];

        A(4,0) = 0; A(4,1) = 0; A(4,2) = 0; A(4,3) = bx[1]; A(4,4) = by[1]; A(4,5) = 1; A(4,6) = -bx[1] * ay[1]; A(4,7) = -by[1] * ay[1];

        A(5,0) = 0; A(5,1) = 0; A(5,2) = 0; A(5,3) = bx[2]; A(5,4) = by[2]; A(5,5) = 1; A(5,6) = -bx[2] * ay[2]; A(5,7) = -by[2] * ay[2];

        A(6,0) = 0; A(6,1) = 0; A(6,2) = 0; A(6,3) = bx[3]; A(6,4) = by[3]; A(6,5) = 1; A(6,6) = -bx[3] * ay[3]; A(6,7) = -by[3] * ay[3];

        A(7,0) = bx[3]; A(7,1) = by[3]; A(7,2) = 1; A(7,3) = 0; A(7,4) = 0; A(7,5) = 0; A(7,6) = -bx[3] * ax[3]; A(7,7) = -by[3] * ax[3];


        Vector b(8);

        b[0] = ax[0]; b[1] = ax[1]; b[2] = ax[2]; b[3] = ay[0]; b[4] = ay[1]; b[5] = ay[2]; b[6] = ay[3]; b[7] = ax[3];


        // Estimate homography
        Vector h(8);
        h = b/A;

        Matrix H(3,3); // Estimated homography matrix
        H(0,0) = h[0]; H(0,1) = h[1]; H(0,2) = h[2]; H(1,0) = h[3]; H(1,1) = h[4]; H(1,2) = h[5]; H(2,0) = h[6]; H(2,1) = h[7]; H(2,2) = 1;

        //std::cout << "H:" << std::endl; H.print();

        // Check against all other features
        for (int j = 0; j < nMatches; j++) {

            if (j == ft[0] || j == ft[1] || j == ft[2] || j == ft[3]) continue; // Skip features used in estimation

            // Push this through estimated homography and see how much error to ground truth
            Vector t(3); // Test point
            Vector a(3); // Actual, ground truth

            // Estimate x translation
            a[0] = keypoints1[matches[j].queryIdx].pt.x; // New
            t[0] = keypoints2[matches[j].trainIdx].pt.x; // Curr

            // Estimate y translation
            a[1] = keypoints1[matches[j].queryIdx].pt.y;
            t[1] = keypoints2[matches[j].trainIdx].pt.y;

            a[2] = t[2] = 1; // Homogenized

            Vector e(3); // Estimation via homography
            e = H * t;

            Vector diff(3); diff = e - a;

            // Check against ground truth
            double error = L2Norm(diff);
            //std::cout << "error: " << error << std::endl;
            if(error < tolerance) concensus++;

        }

        if(concensus > maxConcensus){
            //std::cout << "ever?" << std::endl;
            maxConcensus = concensus;
            bestHomography = H;
        }

    }

    //std::cout << "best H:" << std::endl; bestHomography.print();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            homography.at<double>(i,j) = -bestHomography(i,j);
        }
    }

    return 0;

}


// Blend 2 images together, both should have same size/bounding box
// Current implementation assumes only blending with left neighbor
// -----------------------------------------------------------------------------
int RadialStitcher::blend(cv::Mat& newImage, cv::Mat& canvas, cv::Mat& newMask, cv::Mat& canvasMask){

    /*
    imshow("newImage", newImage);
    waitKey(0);
    imshow("pano", canvas);
    waitKey(0);
    imshow("newMask", newMask);
    waitKey(0);
    imshow("canvasMask", canvasMask);
    waitKey(0);
    */

    if(newImage.size() != canvas.size()) return -1; // Check same size

    int nRows = canvas.rows;
    int nCols = canvas.cols;

    uchar* rowCan; // Row in canvas/output panorama to update
    uchar* rowNew; // Row in new image to blend in
    double* alphaCan; // Alpha value of canvas/panorama
    double* alphaNew; // Alpha value of new image

    for (int y = 0; y < nRows; y++) {
        rowCan = canvas.ptr<uchar>(y);
        rowNew = newImage.ptr<uchar>(y);
        alphaCan = canvasMask.ptr<double>(y);
        alphaNew = newMask.ptr<double>(y);
        for (int x = 0; x < nCols; x++) {
            if(rowCan[3*x] != 0 && rowNew[3*x] != 0){ // Feather region
                double aN = alphaNew[x];
                double aC = alphaCan[x];
                rowCan[3*x] = (aN * rowNew[3*x] + aC * rowCan[3*x]) / (aN + aC); // Blue
                rowCan[3*x + 1] = (aN * rowNew[3*x + 1] + aC * rowCan[3*x + 1]) / (aN + aC); // Green
                rowCan[3*x + 2] = (aN * rowNew[3*x + 2] + aC * rowCan[3*x + 2]) / (aN + aC); // Red
            } else if (!(rowCan[3*x] > 0)) {
                rowCan[3*x] = rowNew[3*x];
                rowCan[3*x + 1] = rowNew[3*x + 1];
                rowCan[3*x + 2] = rowNew[3*x + 2];
            }
        }
    }

    return 0;

}


// Main stitching process
int RadialStitcher::Stitch(){

    std::cout << "Stitching " << numImages << " images..." << std::endl;

    // Use first image to start panorama
    cv::Mat first = src[0];

    // Output mosaic/canvas
    cv::Mat out = cv::Mat::zeros(1.2 * first.rows, first.cols + ((numImages - 1) * 0.5 * first.cols), first.type());

    // 3x3 Identity
    cv::Mat Id = (cv::Mat_<double>(3,3) <<
        1, 0, 0,
        0, 1, 0,
        0, 0, 1);

    // Center first image
    double xCenter = 0;
    double yCenter = -first.rows/2 + out.rows/2;

    cv::Mat Tr = (cv::Mat_<double>(3,3) << // Translation to center first image of panorama
        1, 0, xCenter,
        0, 1, yCenter,
        0, 0, 1);

    transforms.push_back(Tr); // First homography for first input image

    // Warp in first image
    warpPerspective(first, out, Tr, out.size());

    // Stitch remaining images in relative to first image
    for (int i = 1; i < numImages; i++) {

        cv::Mat curr = src[i]; // Current image to stitch in...
        cv::Mat left = src[(i - 1) % numImages]; // ...And its left neighbor image

        getFeatures(curr, left);

        cv::Mat H = (cv::Mat_<double>(3,3) << // Modify this to relate curr image to neighbor
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);

        estimateHomography(H); // Find translation

        cv::Mat warped = cv::Mat::zeros(out.rows, out.cols, out.type()); // Translated image
        cv::Mat newMask = cv::Mat::zeros(out.rows, out.cols, CV_64F);

        // Use this matrix to find the chain of transformations relating curr
        // image to the first image
        cv::Mat C = (cv::Mat_<double>(3,3) <<
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);

        // Stack all previous translations and...
        C.at<double>(0, 2) += transforms[i - 1].at<double>(0, 2);
        C.at<double>(1, 2) += transforms[i - 1].at<double>(1, 2);

        //...add new homography for curr image
        C.at<double>(0, 2) += H.at<double>(0, 2);
        C.at<double>(1, 2) += H.at<double>(1, 2);

        transforms.push_back(C);
        warpPerspective(curr, warped, C, warped.size()); // Warp curr image
        warpPerspective(blendMasks[i], newMask, C, newMask.size()); // And its mask
        //std::cout << "here?" << std::endl;

        // Get its left neighbor's mask
        cv::Mat prevMask = cv::Mat::zeros(out.rows, out.cols, CV_64F);
        warpPerspective(blendMasks[i - 1], prevMask, transforms[i - 1], prevMask.size());

        // Blend and add to panorama
        //out = 0.5 * warped + 0.5 * out; // Add to panorama
        blend(warped, out, newMask, prevMask);

    }

    cv::imwrite("panorama.jpg", out);
    cv::imshow("panorama", out);
    cv::waitKey(0);

    return 0;

}
