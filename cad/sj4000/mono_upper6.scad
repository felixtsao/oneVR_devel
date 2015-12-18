$fn=200;

distance = 52; // distance from optical center

// trench dimensions
width = 42;
length = 25.7; 
depth = 8;
lift = 3;

difference(){

    // outer radius
    cylinder(h = 10, r = 1.36 * distance, center = true);

    translate([distance, 0, lift])
    cube(size = [length, width, depth], center = true);
      
    rotate([0,0,60])
    translate([distance, 0, lift])
    cube(size = [length, width, depth], center = true);

    rotate([0,0,120])
    translate([distance, 0, lift])
    cube(size = [length, width, depth], center = true);

    rotate([0,0,180])
    translate([distance, 0, lift])
    cube(size = [length, width, depth], center = true);

    rotate([0,0,240])
    translate([distance, 0, lift])
    cube(size = [length, width, depth], center = true);

    rotate([0,0,300])
    translate([distance, 0, lift])
    cube(size = [length, width, depth], center = true);

    // center dip
    translate([0, 0, lift])
    cylinder(h = 8, r = 37, center = true);

    // clearance hole for 1/4 20 bolt
    cylinder(h = 20, r = 3.18, center = true);
}


// model sjcam4000 cameras

/*
translate([-50, 0, 29.5])
rotate([90, 0, 180])
union(){ // sj400cam
    // camera body
    color("dimgray")
    cube(size = [24.7, 60, 41], center = true);

    // lens
    color("deepskyblue")
    translate([12, 16, 6])
    rotate([0,90,0])
    cylinder(h = 5, r = 10, center = true);

    // power button
    color("gray")
    translate([12, -19, 8])
    rotate([0,90,0])
    cylinder(h = 2, r = 5, center = true);

    // ok button
    color("gray")
    translate([0, -19, 21])
    cylinder(h = 2, r = 5, center = true);
}
*/
