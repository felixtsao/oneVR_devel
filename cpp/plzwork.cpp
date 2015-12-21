#include <string>
#include "opencv2/opencv_modules.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/videoio/videoio.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/stitching/detail/autocalib.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/timelapsers.hpp"
#include "opencv2/stitching/detail/camera.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/util.hpp"
#include "opencv2/stitching/detail/warpers.hpp"
#include "opencv2/stitching/warpers.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace cv::detail;

//hide the local functions in an anon namespace
namespace {
    void help(char** av) {
        cout << "The program captures frames from a video file, image sequence (01.jpg, 02.jpg ... 10.jpg) or camera connected to your computer." << endl
             << "Usage:\n" << av[0] << " <video file, image sequence or device number>" << endl
             << "q,Q,esc -- quit" << endl
             << "space   -- save frame" << endl << endl
             << "\tTo capture from a camera pass the device number. To find the device number, try ls /dev/video*" << endl
             << "\texample: " << av[0] << " 0" << endl
             << "\tYou may also pass a video file instead of a device number" << endl
             << "\texample: " << av[0] << " video.avi" << endl
             << "\tYou can also pass the path to an image sequence and OpenCV will treat the sequence just like a video." << endl
             << "\texample: " << av[0] << " right%%02d.jpg" << endl;
    }

    Mat warp(Mat frame_in){

    	Mat frame_out = frame_in;

    	Ptr<WarperCreator> warper_creator;
        //warper_creator = makePtr<cv::SphericalWarper>();
        warper_creator = makePtr<cv::MercatorWarper>();

        float warped_image_scale = 1000;
    	Ptr<RotationWarper> warper = warper_creator->create(warped_image_scale);

    	
		//Mat_<float> K = Mat_<std::complex<float> >(3, 3);
		Mat_<float> K(3, 3);
		Mat_<float> R(3, 3);
		//Mat R(3, 3, CV_32F);
/* Default
		int x, y;
		for (x = 0; x < 3; x++){
			for (y = 0; y < 3; y++){
				K(x, y) = 0.0;
				R(x, y) = 0.0;
			}
		}

		K.at<float> (0,0) = 193.0; K.at<float> (0,2) = 163.5;
		K.at<float> (1,1) = 193.0; K.at<float> (1,2) = 92.0;
		K.at<float> (2,2) = 1.0;
*/

		// Camera Intrisics Matrix K. See OpenCV3 docs for matrix format
		K.at<float> (0,0) = 193.0;   K.at<float> (0,1) = 0.0;    K.at<float> (0,2) = 960.0;
		K.at<float> (1,0) = 0.0;     K.at<float> (1,1) = 193.0;  K.at<float> (1,2) = 540.0;
		K.at<float> (2,0) = 0.0;     K.at<float> (2,1) = 0.0;    K.at<float> (2,2) = 1.0;

		// Rotation Matrix R, give it ID
		R.at<float> (0,0) = 1.0; R.at<float> (0,1) = 0.0; R.at<float> (0,2) = 0.0;
		R.at<float> (1,0) = 0.0; R.at<float> (1,1) = 1.0; R.at<float> (1,2) = 0.0;
		R.at<float> (2,0) = 0.0; R.at<float> (2,1) = 0.0; R.at<float> (2,2) = 1.0;
	
		//cout << "Initial intrinsics #\n" << K << ":\n";
		//cameras[i].K().convertTo(K, CV_32F);    	


    	//vector<Point> corners(1);

    	//warper->warp(frame_in, K, R, INTER_LINEAR, BORDER_CONSTANT, frame_out);
    	warper->warp(frame_in, K, R, INTER_LINEAR, BORDER_CONSTANT, frame_out);


    	return frame_out;
    }

    int process(VideoCapture& capture) {
        int n = 0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        cout << "press space to save a picture. q or esc to quit" << endl;
        namedWindow(window_name, CV_WINDOW_NORMAL); 
        //namedWindow(window_name, WINDOW_KEEPRATIO); //resizable window;

        Mat frame;
        Mat cap;

        for (;;) {
            capture >> cap;
            if (cap.empty())
                break;

            frame = warp(cap);
            //frame = cap;

            imshow(window_name, frame);
            char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input

            switch (key) {
            case 'q':
            case 'Q':
            case 27: //escape key
                return 0;
            case ' ': //Save an image
                sprintf(filename,"filename%.3d.jpg",n++);
                imwrite(filename,frame);
                cout << "Saved " << filename << endl;
                break;
            default:
                break;
            }
        }
        sprintf(filename,"filename%.3d.jpg",n++);
                imwrite(filename,frame);
                cout << "Saved " << filename << endl;
        return 0;
    }
}

int main(int ac, char** av) {

    if (ac != 2) {
        help(av);
        return 1;
    }
    std::string arg = av[1];
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
        help(av);
        return 1;
    }
    return process(capture);
}