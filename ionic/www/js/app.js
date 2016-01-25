// Ionic Starter App

// angular.module is a global place for creating, registering and retrieving Angular modules
// 'starter' is the name of this angular module example (also set in a <body> attribute in index.html)
// the 2nd parameter is an array of 'requires'
angular.module('starter',
    [
        'ionic',
        'app.controllers',
        'app.services',
        'user.controllers',
        'user.services'
    ]
)
/**
 * see documentation: https://www.parse.com/apps/quickstart#parse_data/web/existing
 *
 * SET THESE VALUES IF YOU WANT TO USE PARSE, COMMENT THEM OUT TO USE THE DEFAULT
 * SERVICE
 *
 * parse constants
 */

    .value('ParseConfiguration', {
        applicationId: "OmsGMVVEXlyeH7ogBUCxYBYrhTxskALiSyUI3NQ4",
        javascriptKey: "IxFoJ67sWnY97WVgFPSpesiXhccPLWEdrCs3EMuj"
    })
/**
 *
 */
    .config(function ($stateProvider, $urlRouterProvider) {

        // Learn more here: https://github.com/angular-ui/ui-router
        // Set up the various states which the app can be in.
        // Each state's controller can be found in controllers.js
        $stateProvider
            // create account state
            .state('app-signup', {
                url: "/signup",
                templateUrl: "templates/user/signup.html",
                controller: "SignUpController"
            })
            // login state that is needed to log the user in after logout
            // or if there is no user object available
            .state('app-login', {
                url: "/login",
                templateUrl: "templates/user/login.html",
                controller: "LoginController"
            })

            // setup an abstract state for the tabs directive, check for a user
            // object here is the resolve, if there is no user then redirect the
            // user back to login state on the changeStateError
            .state('tab', {
                url: "/tab",
                abstract: true,
                templateUrl: "templates/tabs.html",
                resolve: {
                    user: function (UserService) {
                        var value = UserService.init();
                        return value;
                    }
                }
            })

            // Each tab has its own nav history stack:

            .state('tab.upload', {
                url: '/upload',
                views: {
                    'tab-upload': {
                        templateUrl: 'templates/tab-upload.html',
                        controller: 'ListCtrl'
                    }
                }
            })

            .state('tab.list', {
                url: '/list',
                views: {
                    'tab-list': {
                        templateUrl: 'templates/tab-list.html',
                        controller: 'ListCtrl'
                    }
                }
            })

            .state('tab.list-detail', {
                url: '/list/:itemId',
                views: {
                    'tab-list': {
                        templateUrl: 'templates/cardboardView.html',
                        controller: 'CardboardCtrl'
                    }
                }
            })

            .state('tab.account', {
                url: '/account',
                cache: false,
                views: {
                    'tab-account': {
                        templateUrl: 'templates/tab-account.html',
                        controller: 'AccountCtrl'
                    }
                }
            });

        // if none of the above states are matched, use this as the fallback
        $urlRouterProvider.otherwise('/tab/list');

    })
    .run(function ($ionicPlatform, $rootScope, $state) {


        $rootScope.$on('$stateChangeError',
            function (event, toState, toParams, fromState, fromParams, error) {

                debugger;

                console.log('$stateChangeError ' + error && (error.debug || error.message || error));

                // if the error is "noUser" the go to login state
                if (error && error.error === "noUser") {
                    event.preventDefault();

                    $state.go('app-login', {});
                }
            });

        $ionicPlatform.ready(function () {
            // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
            // for form inputs)
            if (window.cordova && window.cordova.plugins.Keyboard) {
                cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
            }
            if (window.StatusBar) {
                StatusBar.styleDefault();
            }
        });
    })

.directive('cardboardGl', [function() {

  return {
    restrict: 'E',
    scope: {
       sphereUrl: '=sphere',
       isPhoto: '=photo'
    },
    link: function($scope, $element, $attr) {
      create($element[0], $scope.sphereUrl, $scope.isPhoto);
    }
  }


  function create(glFrame, sphereUrl, isPhoto) {
    var scene,
        camera,
        renderer,
        element,
        container,
        effect,
        controls,
        clock;

      init();

      if(isPhoto == true){
        console.log("This item is a picture");
      }
      console.log("isPicture value: " + isPhoto);


    console.log(sphereUrl);


    function init() {
      scene = new THREE.Scene();
      camera = new THREE.PerspectiveCamera(90, window.innerWidth / window.innerHeight, 1, 1000);
        camera.position.set(0, 0, 0);

      scene.add(camera);

      renderer = new THREE.WebGLRenderer();
      element = renderer.domElement;
      container = glFrame;
      container.appendChild(element);

      effect = new THREE.StereoEffect(renderer);

      // Our initial control fallback with mouse/touch events in case DeviceOrientation is not enabled
      controls = new THREE.OrbitControls(camera, element);
      controls.target.set(
        camera.position.x,
        camera.position.y,
        camera.position.z + 0.000001
      );
      controls.noPan = true;
      controls.noZoom = true;

      // Our preferred controls via DeviceOrientation
      function setOrientationControls(e) {
        if (!e.alpha) {
          return;
        }

        controls = new THREE.DeviceOrientationControls(camera, true);
        controls.connect();
        controls.update();

        element.addEventListener('click', fullscreen, true);

        window.removeEventListener('deviceorientation', setOrientationControls, true);
      }
      window.addEventListener('deviceorientation', setOrientationControls, true);


      // Lighting
      var ambient = new THREE.AmbientLight( 0xffffff);
      scene.add( ambient );

      //creating a sphere

      if(isPhoto){
        var sphere = new THREE.Mesh(
        new THREE.SphereGeometry(128, 128, 64),
        new THREE.MeshPhongMaterial({
        map: THREE.ImageUtils.loadTexture(sphereUrl)
        })
        );
        sphere.position.set(0,0,0);
        sphere.scale.x = -1;
        scene.add(sphere);
      } else {
    ///////////
    // VIDEO //
    ///////////

    // create the video element
    video = document.createElement( 'video' );
    // video.id = 'video';
    // video.type = ' video/ogg; codecs="theora, vorbis" ';
    video.id = 'video';
    video.src = sphereUrl;
    video.loop = true;
    video.setAttribute('webkit-playsinline', 'webkit-playsinline');
    video.load(); // must call after setting/changing source

    videoImage = document.createElement( 'canvas' );
    videoImage.width = 1920;
    videoImage.height = 1080;

    videoImageContext = videoImage.getContext( '2d' );
    // background color if no video present
    videoImageContext.fillStyle = '#FFFFFF';
    videoImageContext.fillRect( 0, 0, videoImage.width, videoImage.height );

    videoTexture = new THREE.Texture( videoImage );
    videoTexture.minFilter = THREE.LinearFilter;
    videoTexture.magFilter = THREE.LinearFilter;

    var movieMaterial = new THREE.MeshBasicMaterial( { map: videoTexture, overdraw: true, side:THREE.DoubleSide } );
    // the geometry on which the movie will be displayed;
    //      movie image will be scaled to fit these dimensions.
    var movieGeometry = new THREE.SphereGeometry(360, 360, 360);

    var movieScreen = new THREE.Mesh(
        movieGeometry,
        movieMaterial );

    movieScreen.position.set(0,0,0);
    movieScreen.scale.x = -1;
    scene.add(movieScreen);

    video.play();

    }

      clock = new THREE.Clock();

      animate();
    }

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

      renderer.setSize(width, height);
      effect.setSize(width, height);
    }

    function update(dt) {
      resize();

      camera.updateProjectionMatrix();
      controls.update(dt);
    }

    function render(dt) {
      effect.render(scene, camera);
        if(!isPhoto){
            videoImageContext.drawImage( video, 0, 0 );
        }
        if ( videoTexture )
            videoTexture.needsUpdate = true;
    }

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

    function getURL(url, callback) {
      var xmlhttp = new XMLHttpRequest();

      xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState == 4) {
           if (xmlhttp.status == 200){
               callback(JSON.parse(xmlhttp.responseText));
           }
           else {
               console.log('We had an error, status code: ', xmlhttp.status);
           }
        }
      }

      xmlhttp.open('GET', url, true);
      xmlhttp.send();
    }
  }
}]);
