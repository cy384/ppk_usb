// a 3d printable adapter for the pro micro board

use <palm-III-adapter.scad>
use <palm-V-adapter.scad>

module pro_micro_bottom()
{
	union()
	{
		difference()
		{
			cube([36, 21, 6.5], center=true);
			translate([0, 0, 1.5]) { cube([34, 19, 5], center=true); };
			translate([17, 0, 1]) { cube([6, 9.1, 4.5], center=true); };
		}
		
		for (i = [-1, 1], j = [-1, 1])
		{
			translate([i*17, j*9, 0.5]) { cube([2, 2, 3], center=true); };
		}
	}
}

module pro_micro_top_v()
{
	union()
	{
		difference()
		{
			hull()
			{
				translate([0, 10, 0]) { cube([36, 21, 0.4], center=true); };
				translate([0, 5, 5]) { cube([30, 8, 1.5], center=true); };
			}
		
			hull()
			{
				translate([0, 3, 0]) { cube([24, 5, 1.5], center=true); };
				translate([0, 4, 5]) { cube([18.7, 5, 1.5], center=true); };
			}
		}

		translate([0, 0, 15.7]) { rotate([180, 0, 180]) { palm_v_adapter(); }; };
	}
}

module pro_micro_top_iii()
{
	union()
	{
		difference()
		{
			hull()
			{
				cube([36, 21, 0.4], center=true);
				translate([0, 2, 5]) { cube([30, 14, 1.5], center=true); };
			}
		
			hull()
			{
				translate([0, 7, 0]) { cube([24, 5, 1.5], center=true); };
				translate([0, 6, 5]) { cube([18.7, 5, 1.5], center=true); };
			}
		}

		translate([0, 0, 5]) { palm_iii_adapter(); };
	}
}

translate([0, 30, 3.25]) { pro_micro_bottom(); };

translate([0, -40, 0.2]) { pro_micro_top_v(); };

translate([0, 0, 0.2]) { pro_micro_top_iii(); };
