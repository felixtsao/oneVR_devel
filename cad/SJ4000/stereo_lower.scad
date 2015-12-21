$fn=200;

// trench dimensions
width = 41.1;
length = 24.8;
depth = 10;
lift = 3; // shift up z direction
distance = 120; //distance from optical center

lenset = 6; // offset for lens cutout
lenslift = 10;
lensdist = distance + 16;


difference(){

    // apparatus
    cylinder(h = 15, r = 1.2 * distance, center = true);

    // clearance hole for 1/4 - 20 bolt
    rotate([0, 0, 56])
    translate([distance - 40, 0, 0])
    cylinder(h = 30, r = 3.18, center = true);

    rotate([0, 0, 56])
    translate([-60, 0, 0])
    cube(size = [1.5 * distance, 3 * distance, 20], center = true);

    // trench for camera 1
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

    // trench for camera 2
    rotate([0, 0, 22.5])
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

    // trench for camera 3
    rotate([0, 0, 45])
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

    // trench for camera 4
    rotate([0, 0, 67.5])
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

    // trench for camera 5
    rotate([0, 0, 90])
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

    // trench for camera 6
    rotate([0, 0, 112.5])
    union(){
        translate([distance, 0, lift])
        cube(size = [length, width, depth], center = true);

        translate([lensdist, lenset, lenslift])
        rotate([0, 90, 0])
        cylinder(h = 20, r = 12, center = true);
    }

    }

    /*
    // camera model
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