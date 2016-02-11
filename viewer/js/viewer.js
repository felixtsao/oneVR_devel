/*
 * @author Felix Tsao, github.com/felixtsao
 * web360player - A basic JavaScript/WebGL browser 360 VR video/photo player for phones
 */


//////////////////////////////
// VARIABLES AND FILE PATHS //
/////////////////////////////

var scene,
    clock,
    renderer,
    element,
    container,
    projection,
    camera,
    geometry,
    material,
    texture,
    screen,
    video,
    vr,
    pseudo3d,
    mobile;

var video_path = 'video/src.mp4'; // Choose video source here
var image_path = 'img/src.jpg'; // Choose image source here
var projection = 'equirectangular'; // Choose a projection from those available in the switch
var play_video = true; // Set media type: video or photo

// LAUNCH WEB APP //
////////////////////
main();           //
animate();        //
////////////////////


function main(){

    /////////////////////////////
    // PLAYER / 3D-SCENE SETUP //
    /////////////////////////////

    scene = new THREE.Scene();
    renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    element = renderer.domElement;
    container = document.body
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
    mobile = isMobile.any();
    // ...or lock device type and comment-out above line
    // var mobile = true;

    // Setup user-interaction controls by detected/set device type
    if (mobile){ // Use Gyro-sensor for mobile phones
        camera = init_camera();
    	controls = new THREE.DeviceOrientationControls(camera, true);
    	controls.connect();
    	controls.update();
    	vr = confirm('Enable VR Headset Mode?'); // Ask user
    	element.addEventListener('click', fullscreen, true);
    } else { // Click/drag controls for desktop/laptop
        camera = init_camera();
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
    if (vr){
    	pseudo3d = new THREE.StereoEffect(renderer);
    }


    ///////////////////////
    // PROJECTION CONFIG //
    ///////////////////////

    switch(projection){
    	case 'equirectangular':
    		geometry = new THREE.SphereGeometry(500, 60, 40);
            geometry.scale(-1, 1, 1); // Flip since viewing from inside of sphere
    		break;
    	case 'cube':
            // TODO
            //geometry = new THREE.BoxGeometry(400, 400, 400);
    		break;
    }


    ///////////////////////////
    // VIEWING SCREEN CONFIG //
    ///////////////////////////

    if(play_video){
        init_video_screen();
    } else {
        init_photo_screen();
    }

    clock = new THREE.Clock(); // Spin-up a clock for rendering

}


//////////////////////
// RENDERING CONFIG //
//////////////////////

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
	if (vr){
		pseudo3d.setSize(window.innerWidth, window.innerHeight);
	}
}


function update(dt) {
	resize();
	camera.updateProjectionMatrix();
	controls.update(dt);
}


function render(dt){
	renderer.render(scene, camera);
    if (vr){
		pseudo3d.render(scene, camera);
	}
}


function init_camera(){
    var new_camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 1100);
    new_camera.position.set(0, 0, 0);
    new_camera.target = new THREE.Vector3(0, 0, 0);
    scene.add(new_camera);
    return new_camera;
}


function init_photo_screen(){
    texture = {map: new THREE.TextureLoader().load(image_path)}
	material = new THREE.MeshBasicMaterial(texture);
	var screen = new THREE.Mesh(geometry, material);
	screen.position.set(0,0,0);
	scene.add(screen);
}


function init_video_screen(){
    video = document.createElement('video');
    video.src = video_path;

    video.load();
    video.loop = true;
    video.setAttribute('webkit-playsinline', 'webkit-playsinline');

    var texture = new THREE.VideoTexture(video);
    texture.minFilter = THREE.LinearFilter;
    texture.format = THREE.RGBFormat;
    texture.generateMipmaps = false;

    var material = new THREE.MeshBasicMaterial({map: texture});
    var screen = new THREE.Mesh(geometry, material);
    screen.position.set(0,0,0);
    scene.add(screen);
    video.play();
}


// Mobile Device Fullscreen
function fullscreen(){
	if (container.requestFullscreen){
		container.requestFullscreen();
	} else if (container.msRequestFullscreen){
		container.msRequestFullscreen();
	} else if (container.mozRequestFullScreen){
		container.mozRequestFullScreen();
	} else if (container.webkitRequestFullscreen){
		container.webkitRequestFullscreen();
	}
}
