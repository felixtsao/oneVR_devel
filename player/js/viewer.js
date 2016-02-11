/*
 * @author Felix Tsao, github.com/felixtsao
 * web360player - A basic JavaScript/WebGL browser 360 VR video/photo player (UV Sphere Equirectangular Projection)
 */


/////////////////////////////
// PLAYER / 3D-SCENE SETUP //
/////////////////////////////

var scene = new THREE.Scene();

// Virtual Camera
var camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(0, 0, 0);
scene.add(camera);

// Renderer Setup
var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
var element = renderer.domElement;
var container = document.body
container.appendChild(element);


///////////////////////////
// VIEWING DEVICE CONFIG //
///////////////////////////

// Recognizable mobile devices
var isMobile = {
    Android: function() {
        return navigator.userAgent.match(/Android/i);
    },
    BlackBerry: function() {
        return navigator.userAgent.match(/BlackBerry/i);
    },
    iOS: function() {
        return navigator.userAgent.match(/iPhone|iPad|iPod/i);
    },
    Opera: function() {
        return navigator.userAgent.match(/Opera Mini/i);
    },
    Windows: function() {
        return navigator.userAgent.match(/IEMobile/i);
    },
    any: function() {
        return (isMobile.Android() || isMobile.BlackBerry() || isMobile.iOS() || isMobile.Opera() || isMobile.Windows());
    }
};

// Auto-detect if mobile device or desktop/laptop
var mobile = isMobile.any();
// ...or lock device type and comment-out above line
// var mobile = true;

// Set media type: video or photo
var video = false; 

// Setup user-interaction controls by detected/set device type
if (mobile){ // Use Gyro-sensor for mobile phones
	controls = new THREE.DeviceOrientationControls(camera, true);
	controls.connect();
	controls.update();
	var enable_vr = confirm('Enable VR Headset Mode?'); // Ask user
	element.addEventListener('click', fullscreen, true);
} else { // Click/drag controls for desktop/laptop
	controls = new THREE.OrbitControls(camera, element);
	controls.target.set(
		camera.position.x - 0.0001, // BUG: No idea why an offset is needed for click/drag to work
		camera.position.y,
		camera.position.z
	);
	controls.noPan = true;
	controls.noZoom = true;
}

// Pseudo-stereo3D mode for monoscopic image/video source
if (enable_vr){
	var stereo = new THREE.StereoEffect(renderer);
}


///////////////////////
// PROJECTION CONFIG //
///////////////////////

// Choose projection from those available in the switch
var projection = 'equirectangular';

switch(projection){
	case 'equirectangular':
		var geometry = new THREE.SphereGeometry(1, 32, 32);
		break;
	case 'cubic':
		var geometry = new THREE.BoxGeometry(1, 1, 1);
		break;
}

if(video){ // 360video player setup

	// Import video
    video = document.createElement('video');
    video.id = 'video';

    // VIDEO SOURCE ////////////////
    video.src = 'video/cap1080.mp4';
    ////////////////////////////////

    video.load();
    video.loop = true;
    video.setAttribute('webkit-playsinline', 'webkit-playsinline');

    // Make texture with video
    videoImage = document.createElement( 'canvas' );
    videoImage.width = 1920;
    videoImage.height = 1080;

    videoImageContext = videoImage.getContext( '2d' );
    videoImageContext.fillStyle = '#FFFFFF'; // background color if no video present
    videoImageContext.fillRect( 0, 0, videoImage.width, videoImage.height );

    videoTexture = new THREE.Texture( videoImage );
    videoTexture.minFilter = THREE.LinearFilter;
    videoTexture.magFilter = THREE.LinearFilter;

    var movieMaterial = new THREE.MeshBasicMaterial({ map: videoTexture });
    var movieScreen = new THREE.Mesh(geometry, movieMaterial);
    movieScreen.position.set(0,0,0);
    movieScreen.scale.x = -1;
    scene.add(movieScreen);
    video.play();

} else { // 360photo player setup

	// IMAGE SOURCE////////////////////////////////////////////////////////////////////////////////////
	var image = new THREE.MeshBasicMaterial({map: THREE.ImageUtils.loadTexture('img/curiosity.jpeg')});
	///////////////////////////////////////////////////////////////////////////////////////////////////

	var screen = new THREE.Mesh(geometry, image);
	screen.position.set(0,0,0);
	screen.scale.x = -1;
	scene.add(screen);

}


//////////////////////
// RENDERING CONFIG //
//////////////////////

var clock = new THREE.Clock();
animate();


function animate() {
	requestAnimationFrame(animate);
	update(clock.getDelta());
	render(clock.getDelta());
}

function resize() {
	var width = container.offsetWidth;
	var height = container.offsetHeight;

	camera.aspect = width / height;
	camera.updateProjectionMatrix();

	renderer.setSize(window.innerWidth, window.innerHeight);

	if (enable_vr){
		stereo.setSize(window.innerWidth, window.innerHeight);
	}
}

function update(dt) {
	resize();
	camera.updateProjectionMatrix();
	controls.update(dt);
}

function render(dt){
	renderer.render(scene, camera);

	if(video){
		videoImageContext.drawImage(video, 0, 0);
		videoTexture.needsUpdate = true;
	}

    if (enable_vr){
		stereo.render(scene, camera);
	}
}

// Mobile Device Fullscreen
function fullscreen() {
	if (container.requestFullscreen) {
		container.requestFullscreen();
	} else if (container.msRequestFullscreen) {
		container.msRequestFullscreen();
	} else if (container.mozRequestFullScreen) {
		container.mozRequestFullScreen();
	} else if (container.webkitRequestFullscreen) {
		container.webkitRequestFullscreen();
	}
}
