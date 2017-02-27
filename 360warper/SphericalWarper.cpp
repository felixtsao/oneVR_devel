#define cimg_use_png
#define cimg_use_jpeg

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <omp.h>
#include "SphericalWarper.hpp"
#include "CImg.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286


SphericalWarper::SphericalWarper(){}
SphericalWarper::~SphericalWarper(){ delete output; } // Cleanup heap


SphericalWarper::SphericalWarper(std::string fileName, int outputWidth, double fovD, double r, bool chooseUp, double k1, double k2, double k3, bool clip, int debug){


    std::string path = "temp/input/";
    fileName.insert(0, path);

    // Get input image specs
    cimg_library::CImg<unsigned char> ref(fileName.c_str());
    inputWidth = ref.width();
    inputHeight = ref.height();
    inputSize = inputWidth * inputHeight;
    inputMaxDim = fmax(inputWidth, inputHeight);

    // Setup output image
    this->outputWidth = outputWidth; // Output resolution must be 2:1 ratio
    outputHeight = outputWidth / 2; // Output resolution must be 2:1 ratio
    outputSize = outputWidth * outputHeight; // Total number of pixels
    //output = new cimg_library::CImg<unsigned char>(outputWidth, outputHeight, 1, 3, 0);

    // User specified FOV in degrees, should not exceed 170
    fovR = PI * fovD / 180; // Convert it to radians
    f = (inputMaxDim / 2) / tan(fovR / 2); // Focal length

    this->rotation = r;
    this->chooseUp = chooseUp; // 1 for project north pole of sphere, 0 project side of sphere

    // Lens distortion parameters
    this->k1 = k1; //c / 10; // Affects quadratic term
    this->k2 = k2; // Affects quartic term, more drastic distortion correction
    this->k3 = k3; // Affects hexic(?) term, even more drastic distortion correction for fringes

    this->clip = clip;
    this->debug = debug;

    // Center coordinates of output image
    xCenter = outputWidth / 2;
    yCenter = outputHeight / 2;

    // Setup mask for clipping if chosen in settings
    // Also reduces loop dimensions in warping actual image
    mask = new bool[outputSize];
    lut = new double[2 * outputSize];

    // Initialize minimum bounding box
    xMin = outputWidth; xMax = 0; yMin = outputHeight; yMax = 0;

    // Inverse spherical projection
    #pragma omp parallel for // be sure to link -fopenmp if using parallel processing
    for (int y = 0; y < outputHeight; y++) {
        for (int x = 0; x < outputWidth; x++) {

            // Given a pixel (x, y) from out image, we have it's position on a sphere (xs, ys, zs) in 3D space
            // as well as it horizontal angle (theta) and vertical angle (phi) from the center
            //double theta = (x - xCenter) / f;
            //double phi = (y - yCenter) / f;
            double theta = 2 * PI * (x - xCenter) / outputWidth;
            double phi = PI * (y - yCenter) / outputHeight;

            double xs = sin(theta) * cos(phi);
            double ys = sin(phi);
            double zs = cos(theta) * cos(phi);

            double xIn, yIn; // Find (xIn, yIn) pixel coordinates from input image to copy color from
            double a = r * PI / 180; // Rotation angle in radians

            // For every pixel in spherical projection (mask), set true if input will map to that pixel
            if(chooseUp){ // Projecting up or side

                if(ys > 0) continue; // Cull backside

                // Apply rotation around axis looking down (ys)
                double xp = xs * cos(a) - zs * sin(a);
                double yp = ys;
                double zp = xs * sin(a) + zs * cos(a);

                // Build clipping mask
                xIn = (f * xp / yp) + (inputWidth / 2);
                yIn = (f * zp / yp) + (inputHeight / 2);

                if(xIn < 0 || xIn >= inputWidth || yIn < 0 || yIn >= inputHeight) continue;
                mask[y * outputWidth + x] = true;

                // Minimum bounding box
                if(x < xMin) xMin = x;
                if(x > xMax) xMax = x;
                if(y < yMin) yMin = y;
                if(y > yMax) yMax = y;

                // Compute projection and radial distortion correction
                xp = xp / yp;
                zp = zp / yp;
                double r2 = (xp * xp) + (zp * zp);
                double xpp = xp / (1 + (k1 * r2) + (k2 * r2 * r2) + (k3 * r2 * r2 * r2));
                double zpp = zp / (1 + (k1 * r2) + (k2 * r2 * r2) + (k3 * r2 * r2 * r2));
                double xmod = xpp - xp;
                double zmod = zpp - zp;
                if((xmod * xmod + zmod * zmod) > 1) continue; // If radial distortion displacement greater than 1 pixels, distortion is wrong
                xIn = (f * xpp) + (inputWidth / 2);
                yIn = (f * zpp) + (inputHeight / 2);

            } else {

                if(zs < 0) continue; // Cull backside

                // Apply rotation around axis looking down (zs)
                double xp = xs * cos(a) - ys * sin(a);
                double yp = xs * sin(a) + ys * cos(a);
                double zp = zs;

                // Basic projection with no distortion correction for generating mask
                xIn = (f * xp / zp) + (inputWidth / 2);
                yIn = (f * yp / zp) + (inputHeight / 2);

                if(xIn < 0 || xIn >= inputWidth || yIn < 0 || yIn >= inputHeight) continue;
                mask[y * outputWidth + x] = true;

                // Minimum bounding box
                if(x < xMin) xMin = x;
                if(x > xMax) xMax = x;
                if(y < yMin) yMin = y;
                if(y > yMax) yMax = y;

                // Compute projection and radial distortion correction
                xp = xp / zp;
                yp = yp / zp;
                double r2 = (xp * xp) + (yp * yp); // Rate of drag towards center
                double xpp = xp / (1 + (k1 * r2) + (k2 * r2 * r2) + (k3 * r2 * r2 * r2));
                double ypp = yp / (1 + (k1 * r2) + (k2 * r2 * r2) + (k3 * r2 * r2 * r2));
                // Eventually, radial compensation will overpower and start distorting backwards
                double xmod = xpp - xp;
                double ymod = ypp - yp;
                if((xmod * xmod + ymod * ymod) > 1) continue; // If radial distortion displacement greater than 1 pixels, distortion is starting to inflect
                xIn = (f * xpp) + (inputWidth / 2);
                yIn = (f * ypp) + (inputHeight / 2);

            }

            // Store projection/distortion correction mapping in a look up table
            // where using output coordinates (x, y) to index into the look up table
            // gives you the coordinate pair of the matching pixel in the input image
            if(xIn >= 0 && xIn < inputWidth && yIn >= 0 && yIn < inputHeight){
                // Interleave (x, y) indices of input image
                lut[y * (2 * outputWidth) + 2 * x] = xIn;
                lut[y * (2 * outputWidth) + 2 * x + 1] = yIn;
            }
        }
    }

    if(clip){ // Clip area of equirectangular projection that input does not map to
        xClip = xMax - xMin;
        yClip = yMax - yMin;
        // Round up resolution to even integer for h.264 resolution constraint
        if(xClip % 2) xClip++;
        if(yClip % 2) yClip++;
        size = xClip * yClip;
        stride = xClip;
        output = new cimg_library::CImg<unsigned char>(xClip, yClip, 1, 3, 0);
    } else {
        size = outputSize;
        stride = outputWidth;
        output = new cimg_library::CImg<unsigned char>(outputWidth, outputHeight, 1, 3, 0);
    }

    std::cout << '\n' << " 360 Warper Assistant v1.0" << '\n';
    std::cout << "--------------------------------------------------------------------------------" << '\n';
    std::cout << '\n';
    std::cout << "    Input Resolution:                    " << inputWidth << " x " << inputHeight << '\n';
    std::cout << "    Output Resolution:                   ";
    if(clip){ std::cout << xClip << " x " << yClip << '\n'; }
    else { std::cout << outputWidth << " x " << outputHeight << '\n'; }
    std::cout << "    Target equirectangular resolution:   " << outputWidth << " x " << outputHeight << '\n';
    std::cout << "    Field of View:                       " << fovD << " degrees" << '\n';
    std::cout << "    Focal length:                        " << f << " pixels" << '\n';
    std::cout << "    Rotate input by:                     " << r << " degrees counter-clockwise" << '\n';
    std::cout << "    Projection Target:                   ";
    if(chooseUp){ std::cout << "Top/North Pole" << '\n'; }
    else { std::cout << "Side/Equator" << '\n'; }
    std::cout << "    r^2 distortion correction factor:    " << k1 << '\n';
    std::cout << "    r^4 distortion correction factor:    " << k2 << '\n';
    std::cout << "    r^6 distortion correction factor:    " << k3 << '\n';
    std::cout << "    Clipping non-projected areas:        ";
    if(clip) { std::cout << "Yes" << '\n'; }
    else { std::cout << "No" << '\n'; }
    if(debug){ std::cout << "    Debug mode on, showing latitude/longitude lines every 30 degrees\n"; }
    else { std::cout << "    Debug mode off, not drawing latitude/longitude lines" << '\n'; }
    std::cout << '\n';
    std::cout << "--------------------------------------------------------------------------------" << '\n';
    std::cout << '\n';
    std::cout << "    Warping, please wait... (Press Ctrl+C to force quit)\n";
    std::cout << '\n';

}



int SphericalWarper::warp(std::string fileName){

    std::string inPath = "temp/input/";
    inPath.append(fileName);
    cimg_library::CImg<unsigned char> input(inPath.c_str());

    unsigned char * orig = input.data(); // Pointer to pixel of input image
    unsigned char * mod = output->data(); // Pointer to pixel of output image

    #pragma omp parallel for // Be sure to link -fopenmp if using parallel processing
    for (int y = yMin; y < yMax; y++){
        for (int x = xMin; x < xMax; x++){

            if(!mask[y * outputWidth + x]) continue;

            // Check lookup table which stores spherical and distortion mapping between output/input image
            double xp = lut[y * (2 * outputWidth) + 2 * x];
            double yp = lut[y * (2 * outputWidth) + 2 * x + 1];

            int xIn = (int) floor(xp);
            int yIn = (int) floor(yp);

            // Not trying to alias right nao so bilinear interpolation
            double xHigh = ceil(xp);
            double xLow = floor(xp);
            double yHigh = ceil(yp);
            double yLow = floor(yp);

            // Landing exactly on a sample?
            if(xHigh - xLow == 0) xHigh++;
            if(yHigh - yLow == 0) yHigh++;

            double weights[4]; // Compute weights
            weights[0] = (xp - xLow) * (yp - yLow); // Lower left
            weights[1] = (xHigh - xp) * (yp - yLow); // Lower Right
            weights[2] = (xp - xLow) * (yHigh - yp); // Upper left
            weights[3] = (xHigh - xp) * (yHigh - yp); // Upper right

            if(clip) { y -= yMin; x -= xMin; } // Offset for clipped dimensions

            // Copy color from input image to output image
            for (int ch = 0; ch < 3; ch++) {
                double intensity = weights[0] * orig[(yIn + 1) * inputWidth + (xIn + 1) + ch * inputSize]
                    + weights[1] * orig[(yIn + 1) * inputWidth + xIn + ch * inputSize]
                    + weights[2] * orig[yIn * inputWidth + (xIn + 1) + ch * inputSize]
                    + weights[3] * orig[yIn * inputWidth + xIn + ch * inputSize];
                mod[y * stride + x + ch * size] = (unsigned char) intensity;
            }

            if(clip) { y += yMin; x += xMin; } // Shift back

        }
    }

    if(debug != 0){
        // Debugging: draw latitude circles every 30 degrees
        int step = outputHeight / 6;
        for (int v = 0; v < outputHeight - (step - 1); v += step) {
            for (int u = 0; u < outputWidth; u++) {
                mod[v*outputWidth + u] = 255; // Red
                mod[v*outputWidth + u + outputSize] = 255; // Green
                mod[v*outputWidth + u + 2*outputSize] = 255; // Blue
            }
        }
        // Debugging, draw longitude circles every 30 degrees
        step = outputWidth / 12;
        for (int u = 0; u < outputWidth - (step - 1); u += step) {
            for (int v = 0; v < outputHeight; v++) {
                mod[v * outputWidth + u] = 255; // Red
                mod[v * outputWidth + u + outputSize] = 255; // Green
                mod[v * outputWidth + u + 2 * outputSize] = 255; // Blue
            }
        }
    }

    std::string outPath = "temp/output/";
    outPath.append(fileName.substr(0, fileName.find(".")));
    outPath.append(".jpg");
    output->save(outPath.c_str(), -1);

    /*
    // Display image in X window
    cimg_library::CImgDisplay main_disp(output,"Image");
    while(!main_disp.is_closed()){
        main_disp.wait();
    };
    */

    return 0;

}
