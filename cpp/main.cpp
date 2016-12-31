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

// OpenCV - Feature Descriptors and Image I/O
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

////////////////////////////////////////////////////////////////////////////////


// How to use this program
// -----------------------------------------------------------------------------
int usage(){
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "    ./radStitch <image1> <image2> ... <imageN>" << std::endl;
    std::cout << std::endl;
    std::cout << "Constraints:" << std::endl;
    std::cout << "    1) Stitcher assumes inputs are taken from radially symmetric viewpoints relative to a single optical center" << std::endl;
    std::cout << "    2) Specified adjacent images must overlap and share common features" << std::endl;
    std::cout << std::endl;
    exit(1);
};


// Get feature points for two adjacent images and determines matching pairs
// Uses ORB descriptor under the hood from OpenCV
// -----------------------------------------------------------------------------
int getFeatures(
    cv::Mat& img1, cv::Mat& img2,
    std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2,
    cv::Mat& descriptors1, cv::Mat& descriptors2, std::vector<cv::DMatch>& goodMatches){

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

    detector->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    // Determine matching pairs via Euclidean distance
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Quick calculation of max and min distances between keypoints
    double max_dist = 0; double min_dist = 100;
    for(int i = 0; i < descriptors1.rows; i++){
        double dist = matches[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }

    // Draw only "good" matches i.e. those that are less than 3 * min_dist
    for(int i = 0; i < descriptors1.rows; i++){
        if(matches[i].distance < 3 * min_dist) goodMatches.push_back(matches[i]);
    }

    cv::Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2,
        goodMatches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
        std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    // Localize the object
    /*
    std::vector<cv::Point2f> img1pt;
    std::vector<cv::Point2f> img2pt;
    for(int i = 0; i < (int) goodMatches.size(); i++){
        // Get the keypoints from the good matches
        img1pt.push_back(keypoints1[goodMatches[i].queryIdx].pt);
        img2pt.push_back(keypoints2[goodMatches[i].trainIdx].pt);
    }
    */

    //cv::imshow("Matches", img_matches);
    //imwrite("match.jpg", img_matches);
    //cv::waitKey(0);

    return 0;

}


// Projects an image onto a cylindrical surface with specified focal length
// -----------------------------------------------------------------------------
int projectCylindrical(cv::Mat &I, cv::Mat&O, double f){

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
int projectSpherical(cv::Mat &I, cv::Mat&O, double f){

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
int projectMaskSpherical(cv::Mat &I, cv::Mat&O, double f){

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


// Compute translation homography between 2 adjacent images
// -----------------------------------------------------------------------------
int estimateHomography(
    std::vector<cv::KeyPoint>& kpNew, std::vector<cv::KeyPoint>& kpCur,
    std::vector<cv::DMatch>& matches, cv::Mat& homography){

    int nMatches = (int) matches.size();

    std::cout << nMatches << " feature point matches" << std::endl;

    //int trials = 500;
    int trials = nMatches;
    double xTrans = 0;
    double yTrans = 0;
    double tolerance = 3.0; // pixels
    int xQual = 0, yQual = 0; // 0 for bad, 1 for good, within tolerance
    int concensus = 0;
    int maxConcensus = 0;
    int bestX = 0;
    int bestY = 0;

    srand(0);


    // RANSAC

    if(nMatches < trials) std::cout << "Less matches than trials" << std::endl;

    // Estimate quality is high at around 500 iterations
    for (int i = 0; i < trials; i++) {

        // Inititialize
        xQual = 0;
        yQual = 0;
        concensus = 0;

        int featureNumber = rand() % nMatches; // Choose random feature pair
        //std::cout << "ftNumb " << featureNumber << std::endl;

        // Use as reference
        double xReferenceNew = kpNew[matches[featureNumber].queryIdx].pt.x;
        double xReferenceCur = kpCur[matches[featureNumber].trainIdx].pt.x;
        double yReferenceNew = kpNew[matches[featureNumber].queryIdx].pt.y;
        double yReferenceCur = kpCur[matches[featureNumber].trainIdx].pt.y;

        // Shift estimate
        xTrans = xReferenceCur - xReferenceNew;
        yTrans = yReferenceCur - yReferenceNew;
        //std::cout << "xTrans " << xTrans << std::endl;
        //std::cout << "yTrans " << yTrans << std::endl;

        // Check against all other features
        for (int j = 0; j < nMatches; j++) {

            if (j == featureNumber) continue; // Skip reference feature

            // Estimate x translation
            double xNew = kpNew[matches[j].queryIdx].pt.x;
            double xCur = kpCur[matches[j].trainIdx].pt.x;
            double xEstimate = xCur - xNew;
            //std::cout << "xEstimate " << xEstimate << std::endl;

            // Estimate y translation
            double yNew = kpNew[matches[j].queryIdx].pt.y;
            double yCur = kpCur[matches[j].trainIdx].pt.y;
            double yEstimate = yCur - yNew;
            //std::cout << "yEstimate " << yEstimate << std::endl;

            // Check if estimate is good against ground truth
            //std::cout << "diffX " << fabs(xTrans - xEstimate) << std::endl;
            //std::cout << "diffY " << fabs(yTrans - yEstimate) << std::endl;
            if(fabs(xTrans - xEstimate) < tolerance) xQual = 1;
            if(fabs(yTrans - yEstimate) < tolerance) yQual = 1;
            if(xQual && yQual) concensus++;

        }

        if(concensus > maxConcensus){
            //std::cout << "ever?" << std::endl;
            maxConcensus = concensus;
            bestX = xTrans;
            bestY = yTrans;
        }

    }

    /*
    // Averaging translation
    //matches = 1; // temp override for testing, delete this later
    for (int i = 0; i < nMatches; i++) {

        double xNew = kpNew[matches[i].queryIdx].pt.x;
        double xCur = kpCur[matches[i].trainIdx].pt.x;
        xTrans += xCur - xNew;

        double yNew = kpNew[matches[i].queryIdx].pt.y;
        double yCur = kpCur[matches[i].trainIdx].pt.y;
        yTrans += yCur - yNew;

    }
    */

    // If no good concensus, just pick the most recent reference
    if (bestX == 0)
        bestX = xTrans;
    if (bestY == 0)
        bestY = yTrans;

    homography.at<double>(0, 2) = bestX;
    homography.at<double>(1, 2) = bestY;
    //std::cout << bestX << std::endl;
    //std::cout << bestY << std::endl;

    return 0;

}


// Makes an alpha/feathering mask for a given image
// Pixels in center of image have alpha value 1 and linearly decreases to 0
// towards edges of image
// -----------------------------------------------------------------------------
int buildBlendMask(cv::Mat& img, cv::Mat& mask){

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


// Blend 2 images together, both should have same size/bounding box
// Current implementation assumes only blending with left neighbor
// -----------------------------------------------------------------------------
int blend(cv::Mat& newImage, cv::Mat& canvas, cv::Mat& newMask, cv::Mat& canvasMask){

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


// Projection options
// -----------------------------------------------------------------------------
enum Projection { CYLINDRICAL, SPHERICAL };


// Panorama stitcher program
// -----------------------------------------------------------------------------
int main(int argc, char ** argv) {

    if(argc < 3) usage(); // Need at least 2 images
    int numImages = argc - 1;

    std::cout << "Stitching " << numImages << " images..." << std::endl;

    // Project and store input images ------------------------------------------

    double f = 600; // Camera focal length parameter (unknown units, not mm)
    //f = 2800; // LA skyline lens (300mm ish)
    f = 10000000000; // SF Marin Headlands (Moto G4)
    Projection projection = SPHERICAL;
    std::vector<cv::Mat> src; // Store input images
    std::vector<cv::Mat> blendMasks; // Store alpha channel blend masks for images
    std::vector<cv::Mat> hStack; // Translation matrices for all images rel. to 1st

    for (int i = 0; i < numImages; i++) {

        cv::Mat temp = cv::imread(argv[i + 1], 1);
        cv::Mat mask = cv::Mat::zeros(temp.rows, temp.cols, CV_64F);
        buildBlendMask(temp, mask);

        if(!temp.data) usage(); // Image is legit?

        src.push_back(cv::Mat::zeros(temp.rows, temp.cols, temp.type()));
        blendMasks.push_back(cv::Mat::zeros(mask.rows, mask.cols, mask.type()));

        // Project image and its blend mask
        if(projection == SPHERICAL){
            projectSpherical(temp, src[i], f);
            projectMaskSpherical(mask, blendMasks[i], f);
        } else {
            projectCylindrical(temp, src[i], f);
            projectCylindrical(mask, blendMasks[i], f);
        }
    }

    // Use first image to start panorama
    cv::Mat center = src[0];

    // Output mosaic/canvas
    cv::Mat out = cv::Mat::zeros(1.2 * center.rows, center.cols + ((numImages - 1) * 0.5 * center.cols), center.type());

    // 3x3 Identity
    cv::Mat Id = (cv::Mat_<double>(3,3) <<
        1, 0, 0,
        0, 1, 0,
        0, 0, 1);

    // Center first image
    double xCenter = 0;
    double yCenter = -center.rows/2 + out.rows/2;

    cv::Mat Tr = (cv::Mat_<double>(3,3) << // Translation to center first image of panorama
        1, 0, xCenter,
        0, 1, yCenter,
        0, 0, 1);

    hStack.push_back(Tr); // First homography for first input image

    // Warp in first image
    warpPerspective(center, out, Tr, out.size());

    // Stitch remaining images in relative to first image
    for (int i = 1; i < numImages; i++) {

        cv::Mat curr = src[i]; // Current image to stitch in...
        cv::Mat left = src[(i - 1) % numImages]; // ...And its left neighbor image

        std::vector<cv::KeyPoint> keypoints1; // curr's keypoints
        std::vector<cv::KeyPoint> keypoints2; // neighbors's keypoints
        cv::Mat descriptors1; // curr's
        cv::Mat descriptors2; // neighbor's
        std::vector<cv::DMatch> matches;

        getFeatures(curr, left, keypoints1, keypoints2, descriptors1, descriptors2, matches);

        cv::Mat H = (cv::Mat_<double>(3,3) << // Modify this to relate curr image to neighbor
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);

        estimateHomography(keypoints1, keypoints2, matches, H); // Find translation

        cv::Mat warped = cv::Mat::zeros(out.rows, out.cols, out.type()); // Translated image
        cv::Mat newMask = cv::Mat::zeros(out.rows, out.cols, CV_64F);

        // Use this matrix to find the chain of transformations relating curr
        // image to the first image
        cv::Mat C = (cv::Mat_<double>(3,3) <<
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);

        // Stack all previous translations and...
        C.at<double>(0, 2) += hStack[i - 1].at<double>(0, 2);
        C.at<double>(1, 2) += hStack[i - 1].at<double>(1, 2);

        //...add new homography for curr image
        C.at<double>(0, 2) += H.at<double>(0, 2);
        C.at<double>(1, 2) += H.at<double>(1, 2);

        hStack.push_back(C);
        warpPerspective(curr, warped, C, warped.size()); // Warp curr image
        warpPerspective(blendMasks[i], newMask, C, newMask.size()); // And its mask
        //std::cout << "here?" << std::endl;

        // Get its left neighbor's mask
        cv::Mat prevMask = cv::Mat::zeros(out.rows, out.cols, CV_64F);
        warpPerspective(blendMasks[i - 1], prevMask, hStack[i - 1], prevMask.size());

        // Blend and add to panorama
        //out = 0.5 * warped + 0.5 * out; // Add to panorama
        blend(warped, out, newMask, prevMask);

    }

    cv::imwrite("panorama.jpg", out);

    //cv::imshow("panorama", out);
    //cv::waitKey(0);



    return 0;

}
