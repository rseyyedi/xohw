/*
 * attitude.h
 *
 *  Created on: Aug 11, 2015
 *      Author: sschreiner
 */

#ifndef ATTITUDE_H_
#define ATTITUDE_H_

/*
 * struct to store angle values
 */
typedef struct{
    float theta;
    float phi;
    float psi;
} angles_s;

/*
 * struct to store acc angle values
 */
typedef struct{
    float theta;
    float phi;
} acc_angles_s;

/*
 * struct to store sin and cos angle values
 */
typedef struct{
    float theta_sin;
    float theta_cos;
    float phi_sin;
    float phi_cos;
    float psi_sin;
    float psi_cos;
} sin_cos_s;

void attitude_calculation(void);
angles_s attitude_get_angles(void);
angles_s attitude_get_angle_velocities(void);
sin_cos_s attitude_get_sin_cos_values(void);
void attitude_to_bram(void);

#endif /* ATTITUDE_H_ */
