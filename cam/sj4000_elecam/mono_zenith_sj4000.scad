$fn = 200;


difference(){

union(){
    // stage
    translate([-17.74,-46.4,0])
    cube(size = [46,65,16.5], center = false);
        
    // 6-32 screw hole well
    translate([-20, 0, 8.25])
    cylinder(h = 16.5, r = 5.1, center = true);
    
    // velcro slot 1
    translate([-26, -28.9, 0])
    cube(size = [9,18,16.5], center = false);
    
    // velcro slot 2
    translate([28, -28.9, 0])
    cube(size = [9,18,16.5], center = false);
    
    
}

// sj4000
translate([-15.24,-43.9,12.5])
cube(size = [41.5,60,24.7], center = false);

// io, rocker cutout    
translate([-10.24,-50,12.5])
cube(size = [31,100,24.7], center = false);

// center screw head clearance
cylinder(h = 20, r = 9, center = true);
    
// 6-32 clearance for mounting screws onto upper lid
translate([20, 0, 0])
cylinder(h = 100, r = 2, center = true);
translate([-20, 0, 0])
cylinder(h = 100, r = 2, center = true);

// 6-32 counter-sink
translate([20, 0, 11])
cylinder(h = 6, r = 5.1, center = true);

// velcro cutout 1
translate([-23, -26.9, 12])
cube(size = [5,14,5], center = false);
translate([-27, -26.9, 9])
cube(size = [9,14,5], center = false);

// velcro cutout 2
rotate([0,0,180]){
translate([-34, 12.9, 12])
cube(size = [5,14,5], center = false);
translate([-38, 12.9, 9])
cube(size = [9,14,5], center = false);
}
    

}