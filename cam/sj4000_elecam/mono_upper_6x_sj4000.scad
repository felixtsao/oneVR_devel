$fn=200;

distance = 50; // distance from optical center

// trench dimensions (mm)
// add 1 to dimension for a little wiggle room
width = 42;
length = 25.7; 
depth = 8;
lift = 3;

difference(){

    // outer radius
    cylinder(h = 10, r = 1.36 * distance, center = true);

    // camera 1 trench
    translate([distance, 0, lift]){
        union(){
            cube(size = [length, width, depth], center = true);
            translate([-4.284, 0, 0])
            cube(size = [7, 26, 12], center = true); //rocker buttons
        }

        // directional microphone cutout
        rotate([0,90,0])
        translate([3,0,11])
        cylinder(h = 15, r = 2.5, center = true);
    }

    // camera 2 trench
    rotate([0,0,60])
    translate([distance, 0, lift]){
        union(){
            cube(size = [length, width, depth], center = true);
            translate([-4.284, 0, 0])
            cube(size = [7, 26, 12], center = true); //rocker buttons
        }
        
        // directional microphone cutout
        rotate([0,90,0])
        translate([3,0,11])
        cylinder(h = 15, r = 2.5, center = true);
    }
    
    // camera 3 trench
    rotate([0,0,120])
    translate([distance, 0, lift]){
        union(){
            cube(size = [length, width, depth], center = true);
            translate([-4.284, 0, 0])
            cube(size = [7, 26, 12], center = true); //rocker buttons
        }
        
        // directional microphone cutout
        rotate([0,90,0])
        translate([3,0,11])
        cylinder(h = 15, r = 2.5, center = true);
    }

    // camera 4 trench
    rotate([0,0,180])
    translate([distance, 0, lift]){
        union(){
            cube(size = [length, width, depth], center = true);
            translate([-4.284, 0, 0])
            cube(size = [7, 26, 12], center = true); //rocker buttons
        }
        
        // directional microphone cutout
        rotate([0,90,0])
        translate([3,0,11])
        cylinder(h = 15, r = 2.5, center = true);
    }

    // camera 5 trench
    rotate([0,0,240])
    translate([distance, 0, lift]){
        union(){
            cube(size = [length, width, depth], center = true);
            translate([-4.284, 0, 0])
            cube(size = [7, 26, 12], center = true); //rocker buttons
        }
    
        // directional microphone cutout
        rotate([0,90,0])
        translate([3,0,11])
        cylinder(h = 15, r = 2.5, center = true);
    }

    // camera 6 trench
    rotate([0,0,300])
    translate([distance, 0, lift]){
        union(){
            cube(size = [length, width, depth], center = true);
            translate([-4.284, 0, 0])
            cube(size = [7, 26, 12], center = true); //rocker buttons
        }
        
        // directional microphone cutout
        rotate([0,90,0])
        translate([3,0,11])
        cylinder(h = 15, r = 2.5, center = true);
    }

    // center dip
    translate([0, 0, lift])
    cylinder(h = 8, r = 30, center = true);

    // clearance hole for 1/4 20 bolt
    cylinder(h = 20, r = 3.5, center = true);
    
    // clearance holes for optional 10/32 top camera bolt
    translate([20, 0, 0])
    cylinder(h = 20, r = 1.90, center = true);
    translate([-20, 0, 0])
    cylinder(h = 20, r = 1.90, center = true);
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
