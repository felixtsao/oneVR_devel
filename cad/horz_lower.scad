$fn=200;


difference(){

difference(){

cylinder(h = 12, r = 72, center = true);

translate([56, 16, 2])
cube(size = [24.7,59.5,10], center = true);

rotate([0,0,60])
translate([56, 16, 2])
cube(size = [24.7,59.5,10], center = true);

rotate([0,0,120])
translate([56, 16, 2])
cube(size = [24.7,59.5,10], center = true);

rotate([0,0,180])
translate([56, 16, 2])
cube(size = [24.7,59.5,10], center = true);

rotate([0,0,240])
translate([56, 16, 2])
cube(size = [24.7,59.5,10], center = true);

rotate([0,0,300])
translate([56, 16, 2])
cube(size = [24.7,59.5,10], center = true);

}


translate([0, 0, 5])
cylinder(h = 10, r = 35, center = true);

// bolt
translate([0, 0, 13.5])
cylinder(h = 80, r = 2.6, center = true);
}

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
translate([-23, -32, -68])
sphere(r = 10, center = true);

// ok button
color("gray")
rotate([0,0,0])
translate([-56, 1.5, 38])
cylinder(h = 2, r = 5, center = true);
