// a 3d printable adapter for palm-III era devices

module main_hull()
{
	cube(size = [30.00, 10.00, 10.00], center = true);
}

module socket()
{
	translate ([0, 0, 3.30])
	{
		cube(size = [26.50, 7.00, 15.00], center = true);
	};
}

module pin()
{
	translate([-1.95/2, 0, 0])
	{
		cube(size = [1.95, 1.00, 10.00], center = false);
	}

	translate([(-1.95/2)-0.4, -0.40, 0])
	{
		cube(size = [0.80, 5.80, 10.00], center = false);
	}

	translate([(+1.95/2)-0.4, -0.40, 0])
	{
		cube(size = [0.80, 5.80, 10.00], center = false);
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

module palm_iii_adapter()
{
	difference()
	{
		union()
		{
			// main body
			translate([0, 0, 5.00]) difference()
			{
				main_hull();
				socket();
			}

			// space for each pin
			for (i = [0 : 4])
			{
				translate([-((1.95/2)+(1.95*i)), 3.5, 0]) pin();
				translate([(1.95/2)+(1.95*i), 3.5, 0]) pin();
			}
		}

		// make holes for the contacts
		for (i = [0 : 4])
		{
			translate([-((1.95/2)+(1.95*i)), 0, 0]) hole();
			translate([(1.95/2)+(1.95*i), 0, 0]) hole();
		}
	}
}

palm_iii_adapter();
