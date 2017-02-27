////////////////////////////////////////////////////////////////////////////////
//
// Program: 360warper
// Author: Felix Tsao
// Contact: hello@felixtsao.com
//
// Summary:
//
//     Warper for 360 photo/video stitching
//     Takes an image/sequences and outputs corresponding spherical projection
//     Uses radial model for fixing distortion
//     Assumes a radially symmetric camera model like images shown at
//     (github.com/felixtsao/oneVR_devel)
//     i.e. lateral cameras all in the same plane and single up/down cameras
//
//     User must specify to project input image as up/down camera or side camera
//     and FOV in degrees, and 2 distortion correction paramters. See comments
//     in code.
//
//     This software uses the CImg library and follows the same open-source
//     license that it uses.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <omp.h>
#include "SphericalWarper.hpp"


void usage(){
    std::cout << '\n';
    std::cout << " 360 Warp Assistant v1.0" << '\n';
    std::cout << "--------------------------------------------------------------------------------" << '\n';
    std::cout << '\n';
    std::cout << "    To see the accompanying YouTube tutorial, visit:" << '\n';
    std::cout << "    https://www.youtube.com/watch?v=F78drmyd21I" << '\n';
    std::cout << '\n';
    std::cout << "    This binary is not intended to be run without its python manager script." << '\n';
    std::cout << "    Please run warp.py to warp a video/image file." << '\n';
    std::cout << '\n';
    std::cout << "    The following files must be in the same directory to work." << '\n';
    std::cout << "        warp.py, 360warper, settings.txt and ffmpeg" << '\n';
    std::cout << '\n';
    std::cout << "    Choose warper settings by configuring the 'settings.txt' file." << '\n';
    std::cout << "    Horizontal output resolution (output), field of view (fov), rotation (r)," << '\n';
    std::cout << "    projection direction--side (up 0) or to north pole (up 1)-- and distortion" << '\n';
    std::cout << "    correction constants (k1, k2, k3) must be specified in 'settings.txt'" << '\n';
    std::cout << '\n';
    exit(1);
}


int main(int argc, char ** argv) {

    // Check for key to prevent accidental misuse of program
    if(argc != 2) usage();
    std::string key = argv[1];
    if(key != "-k") usage();

    int width = 4096;
    double fov = -1;
    double r = 0;
    bool chooseUp = false;
    double k1 = 0;
    double k2 = 0;
    double k3 = 0;
    bool clip = false;
    bool debug = false;

    // Get settings from settings.txt file
    std::ifstream settings("settings.txt", std::ios::in);
    if (settings.is_open()){

        std::string buffer, word;

        while (std::getline(settings, buffer)){

            std::stringstream ss(buffer);
            ss >> word;

            if(word == "fov"){
                ss >> word;
                fov = std::stod(word);
                if(fov > 170 || fov < 0) usage();
            } else if (word == "r"){
                ss >> word;
                r = std::stod(word);
            } else if (word == "up"){
                ss >> word;
                chooseUp = (bool) std::stoi(word);
            } else if (word == "k1"){
                ss >> word;
                k1 = std::stod(word);
            } else if (word == "k2"){
                ss >> word;
                k2 = std::stod(word);
            } else if (word == "k3"){
                ss >> word;
                k3 = std::stod(word);
            } else if (word == "resolution"){
                ss >> word;
                width = std::stoi(word);
                if(width < 0) usage();
            } else if (word == "clip"){
                ss >> word;
                clip = (bool) std::stoi(word);
            } else if (word == "debug"){
                ss >> word;
                debug = (bool) std::stoi(word);
            }

        }

        settings.close();

    } else { usage(); }

    std::ifstream frameList("temp/input.txt", std::ios::in);
    if (frameList.is_open()){

        std::string lineIn;
        std::vector<std::string> fileNames;
        while (std::getline(frameList, lineIn)){
            fileNames.push_back(lineIn);
        }

        int nFrames = fileNames.size();

        SphericalWarper * s1 = new SphericalWarper(fileNames[0], width, fov, r, chooseUp, k1, k2, k3, clip, debug);
        //SphericalWarper * s2 = new SphericalWarper(fileNames[0], width, fov, r, chooseUp, k1, k2, k3, clip, debug);
        //SphericalWarper * s3 = new SphericalWarper(fileNames[0], width, fov, r, chooseUp, k1, k2, k3, clip, debug);
        //SphericalWarper * s4 = new SphericalWarper(fileNames[0], width, fov, r, chooseUp, k1, k2, k3, clip, debug);

        for (int i = 0; i < nFrames; i++) {
            s1->warp(fileNames[i]);
        }

        /*
        for (int i = 0; i < nFrames / 4; i += 4) {
            for (int j = 0; j < 4; j++) {
                s1->warp(fileNames[i + j]);
            }
            for (int j = 0; j < 4; j++) {
                s1->save(fileNames[i + j]);
            }
        }
        */

        /*
        // 4 threads
        #pragma omp parallel
        {

            #pragma omp single
            {
                for (int i = 0; i < nFrames / 4; i++) {
                    s1->warp(fileNames[i]);
                }
            }

            #pragma omp single
            {
                for (int i = nFrames / 4; i < nFrames / 2; i++) {
                    s2->warp(fileNames[i]);
                }
            }

            #pragma omp single
            {
                for (int i = nFrames / 2; i < 3 * nFrames / 4; i++) {
                    s3->warp(fileNames[i]);
                }
            }

            #pragma omp single
            {
                for (int i = 3 * nFrames / 4; i < nFrames; i++) {
                    s4->warp(fileNames[i]);
                }
            }

        }
        */

        frameList.close();

    } else { usage(); }


    return 0;

}
