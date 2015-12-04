
// trench dimensions
width = 41.1;
length = 24.8;
depth = 10;
affine = 3; // shift up z direction

right = -22; // right eye offset
left = 22; // left eye offset

distance = 120; //distance from optical center


difference(){

    // apparatus
    cylinder(h = 15, r = 1.2 * distance, center = true);

    translate([-60, 0, 0])
    cube(size = [1.5 * distance, 2.5 * distance, 20], center = true);

    // trenches for stereo pair of cameras
    union(){
        translate([distance, right, affine])
        cube(size = [length, width, depth], center = true);

        translate([distance + 20, right + 8, affine + 6])
        rotate([0, 90, 0])
        cylinder(h = 16, r = 12, center = true);

        translate([distance, left, affine])
        cube(size = [length, width, depth], center = true);
    }

    rotate([0, 0, 45])
    union(){
    translate([distance, right, affine])
    cube(size = [length, width, depth], center = true);

    translate([distance, left, affine])
    cube(size = [length, width, depth], center = true);
    }

    rotate([0, 0, -45])
    union(){
    translate([distance, right, affine])
    cube(size = [length, width, depth], center = true);

    translate([distance, left, affine])
    cube(size = [length, width, depth], center = true);
    }

    }

    translate([distance, right, 30])
    rotate([-90, 0, 0])
    union(){ // sj400cam
    // camera body
    color("dimgray")
    cube(size = [24.7, 60, 41], center = true);

    // lens
    color("deepskyblue")
    translate([12, 16, 7])
    rotate([0,90,0])
    cylinder(h = 5, r = 10, center = true);

    // power button
    color("gray")
    translate([12, -20, 11])
    rotate([0,90,0])
    cylinder(h = 2, r = 5, center = true);

    // ok button
    color("gray")
    translate([0, -20, 21])
    cylinder(h = 2, r = 5, center = true);
    }