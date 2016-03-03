$fn=200;

distance = 50; // distance from optical center

// trench dimensions (mm)
// add 1 to dimension for a little wiggle room
width = 42;
length = 25.7; 
depth = 12;

// lens cutout params
lenset = 5;
lenslift = -10;
lensdist = (distance + 12);

num_cameras = 6; // number of cameras
angular_disp = 360 / num_cameras; // angle between each camera

difference(){

    // apparatus
    cylinder(h = 15, r = 1.36 * distance, center = true);

    // trench holding camera 0
    translate([distance, 0, 3])
    cube(size = [length, width, depth], center = true);
    rotate([0,90,0])
    translate([lenslift, lenset, lensdist])
    cylinder(h = 12, r = 12.5, center = true);
        
    // trench holding camera 1
    rotate([0,0, angular_disp * 1]){
    translate([distance, 0, 3])
    cube(size = [length, width, depth], center = true);
    rotate([0,90,0])
    translate([lenslift, lenset, lensdist])
    cylinder(h = 12, r = 12.5, center = true);
    }

    // trench holding camera 2
    rotate([0,0, angular_disp * 2]){
    translate([distance, 0, 3])
    cube(size = [length, width, depth], center = true);
    rotate([0,90,0])
    translate([lenslift, lenset, lensdist])
    cylinder(h = 12, r = 12.5, center = true);
    }

    // trench holding camera 3
    rotate([0,0, angular_disp * 3]){
    translate([distance, 0, 3])
    cube(size = [length, width, depth], center = true);
    rotate([0,90,0])
    translate([lenslift, lenset, lensdist])
    cylinder(h = 12, r = 12.5, center = true);
    }

    // trench holding camera 4
    rotate([0,0, angular_disp * 4]){
    translate([distance, 0, 3])
    cube(size = [length, width, depth], center = true);
    rotate([0,90,0])
    translate([lenslift, lenset, lensdist])
    cylinder(h = 12, r = 12.5, center = true);
    }

    // trench holding camera 5
    rotate([0,0, angular_disp * 5]){
    translate([distance, 0, 3])
    cube(size = [length, width, depth], center = true);
    rotate([0,90,0])
    translate([lenslift, lenset, lensdist])
    cylinder(h = 12, r = 12.5, center = true);
    }

    // center dip feature
    translate([0, 0, depth - 6])
    cylinder(h = 12, r = 30, center = true);

    // clearance hole for 1/4 20 bolt
    cylinder(h = 30, r = 3.18, center = true);

}


// model sjcam4000 cameras

/*
translate([-50, 0, 27])
rotate([-90, 0, 180])
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