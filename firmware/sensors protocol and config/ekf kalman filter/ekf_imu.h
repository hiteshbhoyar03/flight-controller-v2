#ifndef EKF_IMU_H
#define EKF_IMU_H

#ifdef __cplusplus
extern "C" {
#endif

// Initializes EKF state and covariance matrices
void ekf_init();

// EKF prediction using gyroscope data (rad/s)
void ekf_predict(float gx, float gy, float gz);

// EKF correction using accelerometer data (in g)
void ekf_update_accel(float ax, float ay, float az);

// Returns current orientation quaternion: q_out = [q0, q1, q2, q3]
void get_quaternion(float* q_out);

// Converts quaternion to Euler angles (in degrees)
void quaternion_to_euler_deg(float q[4], float* roll_deg, float* pitch_deg, float* yaw_deg);

// Converts quaternion to Euler angles (in radians)
void quaternion_to_euler_rad(float q[4], float* roll_rad, float* pitch_rad, float* yaw_rad);

// Converts quaternion to 3x3 rotation matrix (row-major)
void quaternion_to_rotation_matrix(float q[4], float R[3][3]);

#ifdef __cplusplus
}
#endif

#endif // EKF_IMU_H
