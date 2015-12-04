$fn=200;

// top

translate([0, 0, 100])
rotate([160,10,0])

difference(){

difference(){

// outer radius
cylinder(h = 10, r = 68, center = true);

translate([50, 0, 3])
cube(size = [24.7,40.95,8], center = true);
    
rotate([0,0,60])
translate([50, 0, 3])
cube(size = [24.7,40.95,8], center = true);

rotate([0,0,120])
translate([50, 0, 3])
cube(size = [24.7,40.95,8], center = true);

rotate([0,0,180])
translate([50, 0, 3])
cube(size = [24.7,40.95,8], center = true);


rotate([0,0,240])
translate([50, 0, 3])
cube(size = [24.7,40.95,8], center = true);


rotate([0,0,300])
translate([50, 0, 3])
cube(size = [24.7,40.95,8], center = true);


}

// center dip
translate([0, 0, 3])
cylinder(h = 8, r = 30, center = true);

// bolt
translate([0, 0, 0])
cylinder(h = 20, r = 2.7, center = true);
}






//bottom

difference(){

difference(){

// outer radius
cylinder(h = 15, r = 68, center = true);

translate([50, 0, 3])
cube(size = [24.7,40.95,12], center = true);
rotate([0,90,0])
translate([-10, -6.4, -62])
cylinder(h = 12, r = 12.5, center = true);
    
rotate([0,0,60]){
translate([50, 0, 3])
cube(size = [24.7,40.95,12], center = true);
rotate([0,90,0])
translate([-10, -6.4, -62])
cylinder(h = 12, r = 12.5, center = true);
}

rotate([0,0,120]){
translate([50, 0, 3])
cube(size = [24.7,40.95,12], center = true);
rotate([0,90,0])
translate([-10, -6.4, -62])
cylinder(h = 12, r = 12.5, center = true);
}

rotate([0,0,180]){
translate([50, 0, 3])
cube(size = [24.7,40.95,12], center = true);
rotate([0,90,0])
translate([-10, -6.4, -62])
cylinder(h = 12, r = 12.5, center = true);
}

rotate([0,0,240]){
translate([50, 0, 3])
cube(size = [24.7,40.95,12], center = true);
rotate([0,90,0])
translate([-10, -6.4, -62])
cylinder(h = 12, r = 12.5, center = true);
}

rotate([0,0,300]){
translate([50, 0, 3])
cube(size = [24.7,40.95,12], center = true);
rotate([0,90,0])
translate([-10, -6.2, -62])
cylinder(h = 12, r = 12.5, center = true);
}

}

// center dip
translate([0, 0, 6])
cylinder(h = 12, r = 30, center = true);

// bolt
translate([0, 0, 0])
cylinder(h = 30, r = 2.7, center = true);
}


// sjcam4000 cameras


// 1st
rotate([90,0,0])
translate([6.5, 65, -17.5])
union () {

// camera body
color("dimgray")
rotate([0,0,180])
translate([56, 16, 17.5])
cube(size = [24.59,59.17,40.95], center = true);

// power button
color("gray")
rotate([0,90,0])
translate([-26, 1.5, -68])
cylinder(h = 2, r = 5, center = true);

// lens
color("deepskyblue")
rotate([0,90,0])
translate([-24, -32, -68])
cylinder(h = 5, r = 10, center = true);

// ok button
color("gray")
rotate([0,0,0])
translate([-56, 1.5, 38])
cylinder(h = 2, r = 5, center = true);

}
/*
// 2nd
rotate([0,0,60])
rotate([90,0,0])
translate([6.5, 43, -17.5])
union () {

// camera body
color("dimgray")
rotate([0,0,180])
translate([56, 16, 17.5])
cube(size = [24.59,59.17,40.95], center = true);

// power button
color("gray")
rotate([0,90,0])
translate([-26, 1.5, -68])
cylinder(h = 2, r = 5, center = true);

// lens
color("deepskyblue")
rotate([0,90,0])
translate([-24, -32, -68])
cylinder(h = 5, r = 10, center = true);

// ok button
color("gray")
rotate([0,0,0])
translate([-56, 1.5, 38])
cylinder(h = 2, r = 5, center = true);

} */


// 3rd
rotate([0,0,120])
rotate([90,0,0])
translate([6.5, 43, -17.5])
union () {

// camera body
color("dimgray")
rotate([0,0,180])
translate([56, 16, 17.5])
cube(size = [24.59,59.17,40.95], center = true);

// power button
color("gray")
rotate([0,90,0])
translate([-26, 1.5, -68])
cylinder(h = 2, r = 5, center = true);

// lens
color("deepskyblue")
rotate([0,90,0])
translate([-24, -32, -68])
cylinder(h = 5, r = 10, center = true);

// ok button
color("gray")
rotate([0,0,0])
translate([-56, 1.5, 38])
cylinder(h = 2, r = 5, center = true);

}