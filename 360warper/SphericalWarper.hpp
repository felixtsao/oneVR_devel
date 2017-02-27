// Uncomment for Mac, comment for linux
#define cimg_use_png
#define cimg_use_jpeg

#include <string>
#include "CImg.h"


class SphericalWarper {

    public:

        SphericalWarper();
        SphericalWarper(std::string fileName, int outputWidth, double fovD, double r, bool chooseUp, double k1, double k2, double k3, bool clip, int debug);
        ~SphericalWarper();

        int warp(std::string fileName);

    private:

        // Input image attributes
        int inputWidth; // x
        int inputHeight; // y
        int inputMaxDim;
        int inputSize;

        // Output image attributes
        int outputWidth; // Default resolution 4096 x 2048
        int outputHeight; // Output resolution must be 2:1 ratio, forced height in constructor
        int outputSize; // Total number of pixels
        int xCenter, yCenter; // Center pixel

        double fovR; // Radians
        double f; // Focal length from target resolution
        double rotation; // Rotation in degrees, counter clock wise
        double k1, k2, k3; // Radial distortion correction parameters

        bool chooseUp; // Choose whether to project to pole of sphere (up) or side of sphere

        bool clip; // Percentage to shrink after distortion correction
        int xClip, yClip;
        int stride, size; // Index offsets for pixels in output image, each row and each color channel
        bool * mask; // Projection mask
        double * lut; // Stores corresponding indices of input image at output image coordinates
        int xMin, xMax, yMin, yMax; // Minimum bounding box from mask

        bool debug; // Flag to draw longitude/latitude lines every 30 degrees

        cimg_library::CImg<unsigned char> * output;


};
