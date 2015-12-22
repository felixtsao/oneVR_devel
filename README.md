# One VR
<h3> A low-cost, end-to-end VR solution concept </h3>
<br>
A virtual reality camera system along with a cross-platform (iOS & Android) mobile VR viewer application, forms an end-to-end VR solution; from capture to viewing.
<p>
Designs based around using off-the-shelf items to create a VR system. 3D printable camera mounts for arbitrary camera dimensions can be easily generated with base parameterized models.

<h3> Hardware </h3>
 * 6x SJ4000 action cameras
 * 3D printable circular camera mount
 * Standard Tripod
 * Standard computer, relatively powerful GPU for image processing preferred
<p>
<center>
<img src="/cad/img_ref/sj4000_6x_01.jpg" alt="VRCAM" width="400px"/>
<img src="/cad/img_ref/sj4000_6x_02.jpg" alt="VRCAM" width="400px"/>
</center>

<h4> About the multi-camera system </h4>
Captures monoscopic video in 360 degrees horizontal and ~170 degrees vertical. Six individual streams are stitched and blended together into a cohesive panoramic video.
<p>
4K Demo Footage: https://www.youtube.com/watch?v=lM7lKqry0ZM

<h3> Software </h3>
 * OpenSCAD for rapid, parameterized prototyping of a 3D printable VR camera mount
 * OpenCV and C++ programming used for determining translations and relative positioning between video streams
 * Ionic Mobile Application Framework for rapid prototyping of a cross-platform 360 photo/video mobile phone viewer 
 * After Effects for rendering out all blended video streams as a large, cohesive rectangular .mp4 video

<img src="/ionic/img_ref/exhibit.jpg" alt="Exhibit" width="820px"/>

<h4> Mobile Application Description </h4>
Exhibit is a hybrid, cross-platform mobile application that explores the new and exciting virtual reality experience of viewing homes, apartments and new properties. With a few swipes, users can navigate an intuitive search feature and instantly find themselves standing in the room they want to see via photosphere or videosphere. Users can look around as if they're actually at the property which is made possible through the sophisticated use of gyroscopic sensors and projective geometry. For sellers, adding or modifying listings happens instantly and easily through a Parse database. From home owners to budding and mature businesses, Exhibit aims to bring the best places, to the best people, in the best way.

<h4> To Preview The App </h4>
 1. Install Ionic View on your iOS or Android mobile phone
 2. Login with user "demo@felixtsao.com" and password "demo"
 3. Tap Exhibit and download files
 4. Tap view and find a listing that interests you
 5. Tap the listing and put your phone into a [cardboard viewer](https://www.google.com/get/cardboard/).
 6. Enjoy your VR experience!
<p>

<h3> 3D Printing </h3>

Camera mount printed on a Makerbot Replicator 2 with Red PLA. The mount takes 4 hours to print if things go smoothly! Messed up twice 1/4 of the way in on the Rostock Max V2 as the mount is relatively large for the printer and would begin curling at the edges from non-uniform heating or something.

<img src="/cad/img_ref/sj4000_6x_03.jpg" alt="SJ4000s" width="420px"/>
<img src="/cad/img_ref/sj4000_6x_makerbot.jpg" alt="Makerbot" width="420px"/>

Current Development
--------------------
Automated stitching process using OpenCV 3.0.0

<h4>Bugs:</h4>
// TODO
