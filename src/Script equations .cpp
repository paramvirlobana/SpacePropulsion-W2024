#include <vector>
#include <iostream>
#include <cmath>
#include "infomatics.h"
// Circular orbit initialization
	// user input replace with cin 
	float R = 100;
	// velocity constants
	float vel_x;
	float vel_y;
	float pos_x;
	float pos_y;
	// Gravity center data
	float pos_x_grav = 800;
	float pos_y_grav = 500;
	float U = 6500;
	// Orbit velocity
	float V = sqrt(U / R);
	// Assume initialization on the x axis on the right field.
	vel_x = 0;
	vel_y = V;
	pos_x = R + pos_x_grav;
	pos_y = pos_y_grav;



// Delta V calculation
	// user input 
	float R_new = 200;
	// velocity targets
	float V_new = sqrt(U / R_new);
	float dV1; // delta V of transfer 1
	float dV2; // delta V of transfer 2
	float vel_apro;
	float vel_peri;
	if (R_new > R) 
	{
		vel_apro = sqrt(U * (2 / R_new - 2 / (R_new + R)));
		vel_peri = sqrt(U * (2 / R - 2 / (R_new + R)));
	}
	if (R_new < R)
	{
		vel_apro = sqrt(U * (2 / R - 2 / (R_new + R)))
		vel_peri = sqrt(U * (2 / R_new - 2 / (R_new + R)))
	}
	dV1 = vel_peri - V;		// sign reflect acceleration or decceleration
	dV2 = V_new - vel_apro;

// Delta application 
	// read from live the step of transfer 1
	float vel_x_R;
	float vel_y_R;
	float pos_x_R;
	float pos_y_R;
	// application of dV1 (maintain vector direction)
	vel_x_R = vel_x_R + vel_x_R / sqrt(vel_x_R ^ 2 + vel_y_R ^ 2) * dV1;
	vel_y_R = vel_y_R + vel_y_R / sqrt(vel_x_R ^ 2 + vel_y_R ^ 2) * dV1;
	// calculate transfer location 2
	// position when in transfer orbit
	float pos_x_tx;
	float pos_y_tx;
		// Check coordinate of transfer 1
	if (pos_x_R = pos_x_grav) // transfer occure on x axis velocity change on y axis
	{
		vel_y_tx = vel_y_tx + dV2;
	}
	else if (pos_y_R = pos_y_grav) // transfer occure on y axis velocity change on x axis
	{
		vel_x_tx = vel_x_tx + dV2;
	}
	else // 2nd transfer occure when the coordinat ratio equals on the opposing quadrants 
	{
		if ((pos_x_tx - pos_x_grav) / (pos_y_tx - pos_y_grav) = (pos_x_R - pos_x_grav) / (pos_y_R - pos_y_grav) ) 
		{
		// application of dV1 (maintain vector direction)
		vel_x_tx = vel_x_tx + vel_x_tx / sqrt(vel_x_tx ^ 2 + vel_y_tx ^ 2) * dV2;
		vel_y_tx = vel_y_tx + vel_y_tx / sqrt(vel_x_tx ^ 2 + vel_y_tx ^ 2) * dV2;
		}
	}
	