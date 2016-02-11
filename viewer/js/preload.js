$(window).load(function() {
	//Preloader
	$('#status').delay(300).fadeOut();
	$('#preloader').delay(300).fadeOut('slow');
    $('#tips').hide();

    if (mobile) { // Tips for fullscreen
        $('#tips').show();
        $('#tips').delay(3000).fadeOut('slow');
    }
})
