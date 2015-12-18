$fn=200;

// trench dimensions
width = 42;
length = 25.7;
depth = 10;
lift = 3; // shift up z direction
distance = 128; //distance from optical center

lenset = 5; // offset for lens cutout
lenslift = 10;
lensdist = distance + 16;

lockm = 1.9;
lockf = 2;
lockh = 5;


// modified from stereo_lower.scad
// other trenches not seen, only trench 3 is

difference(){

    // apparatus
    cylinder(h = 15, r = 1.15 * distance, center = true);

    // left
    rotate([0, 0, 56.25])
    translate([distance/2, distance/2, 0])
    cube(size = [1.7 * distance, distance, 20], center = true);

    // right
    rotate([0, 0, -56.25])
    translate([distance/2, distance/2, 0])
    cube(size = [distance, 1.7 * distance, 20], center = true);

    // back
    rotate([0, 0, 45])
    translate([-60, 0, 0])
    cube(size = [2 * distance, 3 * distance, 20], center = true);

    // inner edge
    cylinder(h = 20, r = 0.7 * distance, center = true);



    // trench for camera 3
    rotate([0, 0, 45])
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        // lock femme front
        translate([distance + 5, -26, 0])
        rotate([90, 0, -11.25])
        cylinder(h = lockh, r = lockf, center = true);

        // lock femme back
        translate([distance - 30, -19, 0])
        rotate([90, 0, -11.25])
        cylinder(h = lockh, r = lockf, center = true);

        // lenslet
        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

}

        // lock male front
        rotate([0, 0, 45])
        translate([distance + 5, 26, 0])
        rotate([90, 0, 11.25])
        cylinder(h = lockh - 2, r = lockm, center = true);

        // lock male back
        rotate([0, 0, 45])
        translate([distance - 30, 19, 0])
        rotate([90, 0, 11.25])
        cylinder(h = lockh - 2, r = lockm, center = true);

/*
    // camera model
    rotate([0, 0, 45])
    translate([distance, 0, 28])
    rotate([-90, 0, 0])
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