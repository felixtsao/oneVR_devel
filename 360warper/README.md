#360warper

<img src="img/doc/umbc.jpg" alt="UMBC" width="900px"/>

A standalone image/video warper created to be used with video compositing tools like After Effects, Blender, Nuke etc. to create 360 video.

To see the accompanying YouTube tutorial visit:
https://www.youtube.com/watch?v=F78drmyd21I

To run warper, open a terminal and change to directory (`cd` command) containing all necessary files i.e.

    warp.py
    360warper
    settings.txt
    ffmpeg (3.2.4 included here, visit ffmpeg.org for more info)
    < Any image/video files to warp >(.mp4, .jpg, .png)

Confirm and change parameters in `settings.txt` file for desired use

When ready to warp images/videos, type in the terminal command:

    ./warp.py video1.mp4 video2.mp4 checkerboard.jpg ...

Warping process will run. May take some time.

Process will automatically create and delete a temporary working directory called `/temp` in the same directory.

Warped videos will be saved as `originalName_warped.mp4` in same directory

Warped images will be saved as `originalName_warped.jpg` in same directory
