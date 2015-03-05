// a 3d printable adapter for palm-III era devices

/*module main_hull()
{
	cube(size = [30.00, 10.00, 10.00], center = true);
}

module socket()
{
	translate ([0, 0, 3.30]) union()
	{
		cube(size = [26.50, 7.00, 15.00], center = true);
		
	};
}
*/
module pin()
{
	translate([-1.95/2, -0.8, 0])
	{
		cube(size = [1.95, 1.20, 10.00], center = false);
	}

	translate([(-1.95/2)-0.4, -2.4, 0])
	{
		cube(size = [0.80, 7.60, 10.00], center = false);
	}

	translate([(+1.95/2)-0.4, -2.4, 0])
	{
		cube(size = [0.80, 7.60, 10.00], center = false);
	}
}

module hole()
{
	translate([0, 5.00, 10.00])
	{
		cube(size = [1.15, 5.00, 3.00], center = true);
	}

	translate([0, 5.00, 0.00])
	{
		cube(size = [1.15, 5.00, 3.00], center = true);
	}
}

module palm_v_adapter()
{
	difference()
	{
		union()
		{
			// space for each pin
			for (i = [0 : 4])
			{
				translate([-((1.95/2)+(1.95*i)), 3.5, 0]) pin();
				translate([(1.95/2)+(1.95*i), 3.5, 0]) pin();
			}

			// ears
			translate ([12.65, 1.90, 7]) cube ([5.0, 1.6, 6], center = true);
			translate ([-12.65, 1.90, 7]) cube ([5.0, 1.6, 6], center = true);
		}

		// make holes for the contacts
		for (i = [0 : 4])
		{
			translate([-((1.95/2)+(1.95*i)), 0, 0]) hole();
			translate([(1.95/2)+(1.95*i), 0, 0]) hole();
		}
	}
}

palm_v_adapter();
