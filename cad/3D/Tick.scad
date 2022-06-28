$fn=15;
panel_Height=300;

// Derived Variables
grid = panel_Height / 16;
panel_Width=grid*10;


//Led spacing parameters
led_side_width=6;
led_depth=4;

//Chamber Parameters
circle_diameter=grid*1.65;
midpoint_spacing=grid*1.65;
chamber_depth=4;

//Top Diffuser Parameters
diffuse_depth=0.75;


//ETC
screw_diameter=2.8;
screw_distance=1.5*grid;

bridging_helper_size=0.2;
bridging_padding = 1;

chanfer = 5;

pcb_width = grid / 2;
pcb_depth = 1.6;

//Show/hide elements
led_layer = true;
chamber = true;
bridge_helpers = true;
diffuse_layer = true;

pcb_mode = false;

if (pcb_mode){
    led_layer = false;
    chamber = false;
    bridge_helpers = false;
    diffuse_layer = false;
    pcb_depth = 0;
    
    pcb_link([0,0,0], 0, true);
}



//Define major points
a = [5 * grid, 14 * grid, 0];
b = [8 * grid, 11 * grid, 0];
c = [8 * grid, 5 * grid, 0];
d = [5 * grid, 2 * grid, 0];
e = [2 * grid, 5 * grid, 0];
f = [2 * grid, 11 * grid, 0];
g = [5 * grid, 8 * grid, 0];
//h = [1 * grid, 1 * grid, 0];

points = [a, b, c, d, e, f, g];

difference(){
    union(){
        //Led Layer
        if (led_layer) difference() {
            cube([panel_Width, panel_Height, led_depth]);
            for(i=points)
            {
                translate(i - [led_side_width/2, led_side_width/2, 0])
                    cube([led_side_width, led_side_width, led_depth], false);
            }
        }
        
        
        //Chamber Layer
        if(chamber){
            translate([0, 0, led_depth]){
                difference() {
                    cube([panel_Width, panel_Height, chamber_depth]);
                    
                    //Horizontal
                    for(i=[a, d, g]) { chamber(i, 0); }
                    
                    //Vertical
                    for(i=[b, c, e, f]) { chamber(i, 90); }
                }
            }
        }
        
        //Diffuse Layer
        if(diffuse_layer){
            translate([0,0,led_depth + chamber_depth]){
                cube([panel_Width, panel_Height, diffuse_depth]);
            }
        }
    }

    //Screw Holes
    screw([screw_distance, screw_distance, 0]);
    screw([panel_Width - screw_distance, panel_Height - screw_distance,0]);
    screw([panel_Width - screw_distance,screw_distance,0]);
    screw([screw_distance,panel_Height - screw_distance,0]);

    side_chanfer([0,0,0], 0);
    side_chanfer([panel_Width, panel_Height-chanfer-chanfer, 0], 180);
}

if (!pcb_mode){
    pcb_link(f, 45);
    pcb_link(a, -45);
    pcb_link(b, 225);
    pcb_link(g, 225);
    pcb_link(e, -45);
    pcb_link(d, 45);
}


module pcb_link(position, zAngle, profile=false){
    if(!profile) { linear_extrude(pcb_depth) pcb_profile(position, zAngle); }
    else { pcb_profile(position, zAngle); }
}

module pcb_profile(position, zAngle){
    pcb_grid = pcb_width/2;
    translate(position) rotate([0,0,zAngle]) difference(){
        hull(){
            translate([distance_formula(a,b), 0, 0]) rotate([0,0,-90]) translate([0,-3*pcb_grid,0]) polygon(points=[[-pcb_grid * 2,0], [-pcb_grid, pcb_grid], [pcb_grid, pcb_grid], [pcb_grid * 2,0]]);
            translate([0,0,0]) rotate([0,0,90]) translate([0,1*pcb_grid,0]) polygon(points=[[-pcb_grid * 2,0], [-pcb_grid, pcb_grid], [pcb_grid, pcb_grid], [pcb_grid * 2,0]]);
        } 
        translate([distance_formula(a,b)/2, 0, 0]) circle(screw_diameter/2);
    }
}

// Fillet
module side_chanfer(position, rotation) {
    total_height = led_depth+chamber_depth+diffuse_depth;
    translate(position + [0,chanfer,total_height - chanfer]) rotate([90,0,90+rotation]) { linear_extrude(height=panel_Width)
        difference(){
            polygon(points=[[-chanfer,chanfer], [-chanfer,0],[0,chanfer]]);
            sector(chanfer, [90,180]);
        }

    }
}

module screw(position, length=(led_depth+chamber_depth)){
    translate(position) cylinder(length, screw_diameter/2, screw_diameter/2, false);
}

module chamber(position, zAngle){
    translate(position){
        rotate([0,0,zAngle]){
            difference(){
                hull(){
                    translate([-midpoint_spacing,0,0]) cylinder(chamber_depth, circle_diameter/2, circle_diameter/2);
                    translate([midpoint_spacing,0,0]) cylinder(chamber_depth, circle_diameter/2, circle_diameter/2);
                }
                if(bridge_helpers){
                    translate([0,0,bridging_helper_size/2]) cube([led_side_width+bridging_padding, circle_diameter, bridging_helper_size], true);
                }
            }
        }
    }
}

// Pillaged from here: https://openhome.cc/eGossip/OpenSCAD/SectorArc.html
module sector(radius, angles) {
    r = radius / cos(180 / $fn);
    step = -360 / $fn;

    points = concat([[0, 0]],
        [for(a = [angles[0] : step : angles[1] - 360]) 
            [r * cos(a), r * sin(a)]
        ],
        [[r * cos(angles[1]), r * sin(angles[1])]]
    );

    difference() {
        circle(radius, $fn = $fn + 20);
        polygon(points);
    }
}

function distance_formula(a,b) = sqrt( (b[0] - a[0])^2 + (b[1] - a[1])^2 );