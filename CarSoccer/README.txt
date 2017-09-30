csci4611 assignment2
	Yu Fang (fangx174 / 5094239)

Project Implementation:
	The gravity used is -9.8, and soccer / car will bounce off the wall,
although sometimes with a very large velocity, the edge of the object will
be out of bound for a short while.

	The way I handle car-wall and ball-way collision is to flip the car / ball
velocity vector by the normal of wall it collide with using -2(d·n)*n. Also I
have the special case for the car hit the ground corners, and handled them same as
the case bouncing back with respect to front / back wall.

	The way I used for "bouncing off" in the car-ball collision case is to
make the ball move far away along the collision-normal until the car and the
ball is no more intersecting. 

	Turning the car with left / right arrow key is handled through maintaining
a speed and angle value in car class, and updated the angle and speed if arrow
keys are hit. Then calculate the velocity vector based on the angle and speed values.




This project can be compiled on CSE lab machines.
