$(window).load(function() {
	//Preloader
	$('#status').delay(300).fadeOut();
	$('#preloader').delay(300).fadeOut('slow');
    $('#tips').fadeOut(1);
    $('#play').click(
        function(){
            $('#play').fadeOut(400);
            video.play();
            if (mobile) { // Tips popup notifying of fullscreen
                $('#tips').delay(500).fadeIn(400);
                $('#tips').delay(1500).fadeOut(1000);
            }
        }
    );
})
