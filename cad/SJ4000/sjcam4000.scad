union(){
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