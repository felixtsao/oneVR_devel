# web360viewer

Basic JavaScript/WebGL 360 video/photo viewer for web browsers. Works best on Chrome.
<p>
[Example](https://cdn.rawgit.com/felixtsao/oneVR_devel/master/viewer/index.html)

<h3> Brief Usage </h3>
Specify desired video/photo in file `/js/viewer.js` or rename video/photo file as `/video/src.mp4` or `/img/src.jpg`
<p>
Player default is on video mode, to view photo, set `var play_video = false;` in the file `/js/viewer.js`
<p>
Player default and current implementation only supports equirectangular projection/media. File `/js/viewer.js` includes switch for selecting projection type for future expansion i.e. cubic, pyramidal etc.
<p>
Launch `index.html` in a web browser to play. Can be hosted online and can be viewed on mobile devices using head mounted displays like cardboard.

<h3> Bugs </h3>
May need to swipe up on the address bar to enable fullscreen on iOS devices.
