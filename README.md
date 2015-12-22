# One VR
<h3> A low-cost, end-to-end VR solution concept </h3>
<p>
A virtual reality camera system along with a cross-platform (iOS & Android) mobile VR viewer application, forms an end-to-end VR solution; from capture to viewing.
<p>
Designs based around using off-the-shelf hardware/software. Parameterized 3D model files allow for easy generation of camera mounts to fit most small, rectangular cameras like GoPro, SJCAM and NoPro.

<h3> Directories </h3>

`/cpp` - Current automated video stitching development <br>
`/cad` - 3D printable model files (`.stl`) and generator files (`.scad`), organized by camera model <br>
`/ionic` - Cross-platform mobile VR viewer application built using the Ionic Framework <br>
`/mlab` - Proprietary MATLAB code for a basic panorama stitching algorithm from a class, for learning and reference


<h3> Hardware/Software </h3>
Listed below are items I have tested but any of them can be swapped out with other items that serve a similar purpose.

<h5> Hardware list </h5>
 * 6x SJ4000 action cameras
 * 3D printable circular camera mount
 * Standard Tripod
 * Standard computer, relatively powerful GPU for image processing preferred
<p>
<center>
<img src="/cad/sj4000/img_ref/sj4000_6x_01.jpg" alt="VRCAM" width="400px"/>
<img src="/cad/sj4000/img_ref/sj4000_6x_02.jpg" alt="VRCAM" width="400px"/>
</center>

<h5> About the multi-camera system </h5>
Captures monoscopic video in 360 degrees horizontal and ~170 degrees vertical. Six individual streams are stitched and blended together into a cohesive panoramic video.
<p>
 6 SJ4000 cameras arranged like the picture below is just barely enough information to create a 360 video and requires a decent amount of user input to stitch in After Effects. Using 6 SJ4000 cameras seated horizontally would help automated stitching a lot at the cost of some vertical field of view. Using more cameras is ideal, but drives up cost and requires a more powerful computer for stitching.
<p>
4K Demo Footage: https://www.youtube.com/watch?v=lM7lKqry0ZM

<h5> Software list </h5>
 * OpenSCAD for rapid, parameterized prototyping of 3D printable VR camera mounts
 * OpenCV and C++ programming for determining translations and relative positioning between video streams
 * Ionic Mobile Application Framework for rapid prototyping of a cross-platform 360 photo/video mobile VR viewer 
 * After Effects and Media Encoder for rendering out all blended video streams as a large, cohesive rectangular .mp4 video

<img src="/ionic/img_ref/exhibit.jpg" alt="Exhibit" width="820px"/>

<h5> Mobile application description </h5>
Exhibit is a hybrid, cross-platform mobile application that explores the new and exciting virtual reality experience of viewing homes, apartments and new properties. With a few swipes, users can navigate an intuitive search feature and instantly find themselves standing in the room they want to see via photosphere or videosphere. Users can look around as if they're actually at the property which is made possible through the sophisticated use of gyroscopic sensors and projective geometry. For sellers, adding or modifying listings happens instantly and easily through a Parse database. From home owners to budding and mature businesses, Exhibit aims to bring the best places, to the best people, in the best way.

<h5> To preview the app </h5>
 1. Install Ionic View on your iOS or Android mobile phone
 2. Login with user "demo@felixtsao.com" and password "demo"
 3. Tap Exhibit and download files
 4. Tap view and find a listing that interests you
 5. Tap the listing and put your phone into a [cardboard viewer](https://www.google.com/get/cardboard/).
 6. Enjoy your VR experience!
<p>

<h3> 3D Printing </h3>

`mono_lower_6x_sj4000.stl` printed on a Makerbot Replicator 2 with Red PLA. The mount takes about 4 hours to print if things go smoothly! Messed up twice 1/4 of the way in on a Rostock Max V2 as the mount is relatively large for that printer's heatbed and would begin curling at the edges from non-uniform heating or something.

<h5> Creating a new camera mount </h5>
Files are located in `/cad` directory, organized by camera model. To create a new camera mount, open the closest existing `.scad` file and adapt it by changing the camera trench dimensions. Camera dimensions are listed as variables at the top of the `.scad` files and should globally change the trench sizes across the mount. To add more cameras, simply increase the distance of the optical center and make additional copies of the trenches and assign each trench with the appropriate angle. The numbers are technically unitless but they default to `mm` for most printers. Be sure to add 1 to the value used for the camera dimension for a little breathing room.

<img src="/cad/sj4000/img_ref/sj4000_6x_03.jpg" alt="SJ4000s" width="420px"/>
<img src="/cad/sj4000/img_ref/sj4000_6x_makerbot.jpg" alt="Makerbot" width="420px"/>

Current Development
--------------------
Automate video stitching process using OpenCV 3.0.0

<h4>Bugs:</h4>
`// TODO`
