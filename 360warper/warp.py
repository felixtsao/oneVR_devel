#!/usr/bin/python

import sys
import os

def bash(cmd):
    return os.system(cmd)
def cd(directory):
    return os.chdir(directory)
def ls(directory):
    return os.listdir(directory)
def exist(fileName):
    return os.path.isfile(fileName)
def usage():
    print ""
    print " 360 Warper Assistant (v1.0)"
    print "--------------------------------------------------------------------------------"
    print ""
    print "    This 360 warper assistant is free to use and free to distribute"
    print ""
    print "    To see the accompanying YouTube tutorial visit:"
    print "        https://www.youtube.com/watch?v=F78drmyd21I"
    print ""
    print "    To run warper, open a terminal and change to directory (cd command)"
    print "    containing all necessary files i.e."
    print ""
    print "        warp.py"
    print "        360warper"
    print "        settings.txt"
    print "        ffmpeg"
    print "        < Any image/video files to warp >(.mp4, .jpg, .png)"
    print ""
    print "    Confirm and change parameters in 'settings.txt' file for desired use"
    print ""
    print "    When ready to warp images/videos, type in the terminal command:"
    print ""
    print "        ./warp.py video1.mp4 video2.mp4 checkerboard.jpg ..."
    print ""
    print "    Warping process will run. May take some time."
    print ""
    print "    Process will automatically create and delete a temporary working directory"
    print "    called '/temp' in the same directory."
    print ""
    print "    Warped videos will be saved as originalName_warped.mp4 in same directory"
    print ""
    print "    Warped images will be saved as originalName_warped.jpg in same directory"
    print ""
    print "   (!) ---- Something went wrong. Please read usage above."
    print ""
    # Cleanup
    cd(startDirectory)
    if(exist("temp")):
        bash("rm -r temp")
    exit(1)

################################################################################

def main(argv):

    global startDirectory
    startDirectory = os.getcwd()

    # Program needs at least 1 input image/video to warp
    if len(argv) < 2:
        usage()

    # Make sure other supporting programs are in same directory
    if(not exist("360warper") or not exist("settings.txt") or not exist("ffmpeg")):
        usage()

    # Get list of files and frame rate from user
    fileNames = []
    for i in range(1, len(argv)):
        if(not exist(argv[i])): # Make sure file is valid and in same directory
            usage()
        fileNames.append(argv[i]) # Add it to list of images/video to warp

    print "    Creating /temp directory"

    bash("mkdir temp") # Working directory, delete at end

    for i in range(0, len(fileNames)):

        bash("mkdir temp/input")
        print "    Invoking ffmpeg to decompress video segment to JPG sequence"
        bash("./ffmpeg -i " + fileNames[i] + " -qscale:v 2 temp/input/%05d.jpg")


        cd("temp")
        imageSequence = ls("input") # Get list of images created by ffmpeg
        imageSequence.sort() # Sort for convenience and readability

        # Store list of images in a file for 360warper to read
        jpgList = open('input.txt', 'w')
        for j in range(0, len(imageSequence)):
            jpgList.write(imageSequence[j] + "\n")
        jpgList.close()

        bash("mkdir output") # Warped images will go in here
        cd("../")

        # Applying spherical warper and distortion correction
        # '-k' is just a key to prevent accidently running 360warper on its own
        bash("./360warper -k")

        # Clear intermediate input files and list
        bash("rm -r temp/input")
        bash("rm temp/input.txt")

        if(len(imageSequence) == 1): # User passed in an image
            dot = fileNames[i].find(".");
            outFileName = fileNames[i][:dot] + "_warped.jpg"
            bash("mv temp/output/00001.jpg ./" + outFileName)
        else: # save mp4 video
            # Get FPS, lol yeah this is pretty whack
            bash("echo \"$(./ffmpeg -i " + fileNames[i] + " 2>&1 | sed -n 's/.*, \(.*\) fp.*/\\1/p')\" > temp/fps.txt")
            fpsFile = open("temp/fps.txt", 'r')
            fps = fpsFile.readline().rstrip()
            fpsFile.close()
            bash("rm temp/fps.txt")
            # Convert warped image sequence to mp4 video
            dot = fileNames[i].find(".");
            outFileName = fileNames[i][:dot] + "_warped.mp4"
            bash("./ffmpeg -r " + fps + " -i temp/output/%05d.jpg -c:v libx264 -profile:v high -crf 18 -pix_fmt yuv420p " + outFileName)

        bash("rm -r temp/output")

    bash("rm -r temp")

    print ""
    print "    (!) ---- Done warping ",
    for i in range(0, len(fileNames)):
        print fileNames[i],
    print "\n"


if __name__ == "__main__":
    main(sys.argv)
