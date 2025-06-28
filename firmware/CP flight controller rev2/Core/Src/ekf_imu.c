// ekf_imu.c
#include "ekf_imu.h"
#include <math.h>
#include <string.h>

#define DT 0.001f  // 1000Hz
#define Q_BIAS 0.0001f  // Process noise variance for the gyro bias estimate
#define Q_ORIENT 0.001f
#define R_ACC 0.01f

static float state[7] = {1, 0, 0, 0, 0, 0, 0};  // q0, q1, q2, q3, bgx, bgy, bgz
static float covariance[7][7] = {0};
static float identity_7x7[7][7];

static void normalize_quat() {
    float norm = sqrtf(state[0]*state[0] + state[1]*state[1] + state[2]*state[2] + state[3]*state[3]);
    for (int i = 0; i < 4; i++) state[i] /= norm;
}

static void quat_predict(float gx, float gy, float gz, float dt) {
    gx -= state[4];
    gy -= state[5];
    gz -= state[6];

    float q0 = state[0], q1 = state[1], q2 = state[2], q3 = state[3];

    float dq0 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    float dq1 = 0.5f * ( q0 * gx + q2 * gz - q3 * gy);
    float dq2 = 0.5f * ( q0 * gy - q1 * gz + q3 * gx);
    float dq3 = 0.5f * ( q0 * gz + q1 * gy - q2 * gx);

    state[0] += dq0 * dt;
    state[1] += dq1 * dt;
    state[2] += dq2 * dt;
    state[3] += dq3 * dt;

    normalize_quat();
}

static void init_identity_matrix(float I[7][7]) {
    memset(I, 0, sizeof(float) * 49);
    for (int i = 0; i < 7; i++) I[i][i] = 1.0f;
}

void ekf_init() {
    memset(covariance, 0, sizeof(covariance));
    for (int i = 0; i < 4; i++) covariance[i][i] = Q_ORIENT;
    for (int i = 4; i < 7; i++) covariance[i][i] = Q_BIAS;
    init_identity_matrix(identity_7x7);
}

void ekf_predict(float gx, float gy, float gz) {
    quat_predict(gx, gy, gz, DT);

    for (int i = 0; i < 4; i++) covariance[i][i] += Q_ORIENT;
    for (int i = 4; i < 7; i++) covariance[i][i] += Q_BIAS;
}

void ekf_update_accel(float ax, float ay, float az) {
    float norm = sqrtf(ax*ax + ay*ay + az*az);
    if (norm < 0.8f || norm > 1.2f) return;

    ax /= norm; ay /= norm; az /= norm;

    float q0 = state[0], q1 = state[1], q2 = state[2], q3 = state[3];

    float gx = 2*(q1*q3 - q0*q2);
    float gy = 2*(q0*q1 + q2*q3);
    float gz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    float residual[3] = {ax - gx, ay - gy, az - gz};

    float H[3][7] = {
        {-2*q2,  2*q3, -2*q0,  2*q1, 0,0,0},
        { 2*q1,  2*q0,  2*q3,  2*q2, 0,0,0},
        { 2*q0, -2*q1, -2*q2,  2*q3, 0,0,0}
    };

    float S[3][3] = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 7; k++) {
                S[i][j] += H[i][k] * covariance[k][k] * H[j][k];
            }
        }
        S[i][i] += R_ACC;
    }

    float K[7][3] = {0};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 7; k++) {
                K[i][j] += covariance[i][k] * H[j][k];
            }
            K[i][j] /= S[j][j];
        }
    }

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            state[i] += K[i][j] * residual[j];
        }
    }

    float KH[7][7] = {0};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            for (int k = 0; k < 3; k++) {
                KH[i][j] += K[i][k] * H[k][j];
            }
        }
    }
    float I_KH[7][7];
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            I_KH[i][j] = identity_7x7[i][j] - KH[i][j];
        }
    }
    float updated_covariance[7][7] = {0};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            for (int k = 0; k < 7; k++) {
                updated_covariance[i][j] += I_KH[i][k] * covariance[k][j];
            }
        }
    }

    memcpy(covariance, updated_covariance, sizeof(covariance));
    normalize_quat();
}

void get_quaternion(float* q_out) {
    q_out[0] = state[0];
    q_out[1] = state[1];
    q_out[2] = state[2];
    q_out[3] = state[3];
}

void quaternion_to_euler_deg(float q[4], float* roll_deg, float* pitch_deg, float* yaw_deg) {
    float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];

    float roll  = atan2f(2*(q0*q1 + q2*q3), 1 - 2*(q1*q1 + q2*q2));
    float pitch = asinf(2*(q0*q2 - q3*q1));
    float yaw   = atan2f(2*(q0*q3 + q1*q2), 1 - 2*(q2*q2 + q3*q3));

    *roll_deg  = roll  * (180.0f / M_PI);
    *pitch_deg = pitch * (180.0f / M_PI);
    *yaw_deg   = yaw   * (180.0f / M_PI);
}

void quaternion_to_euler_rad(float q[4], float* roll_rad, float* pitch_rad, float* yaw_rad) {
    float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];

    *roll_rad  = atan2f(2*(q0*q1 + q2*q3), 1 - 2*(q1*q1 + q2*q2));
    *pitch_rad = asinf(2*(q0*q2 - q3*q1));
    *yaw_rad   = atan2f(2*(q0*q3 + q1*q2), 1 - 2*(q2*q2 + q3*q3));
}

void quaternion_to_rotation_matrix(float q[4], float R[3][3]) {
    float q0 = q[0], q1 = q[1], q2 = q[2], q3 = q[3];

    R[0][0] = 1 - 2*q2*q2 - 2*q3*q3;
    R[0][1] = 2*q1*q2 - 2*q0*q3;
    R[0][2] = 2*q1*q3 + 2*q0*q2;

    R[1][0] = 2*q1*q2 + 2*q0*q3;
    R[1][1] = 1 - 2*q1*q1 - 2*q3*q3;
    R[1][2] = 2*q2*q3 - 2*q0*q1;

    R[2][0] = 2*q1*q3 - 2*q0*q2;
    R[2][1] = 2*q2*q3 + 2*q0*q1;
    R[2][2] = 1 - 2*q1*q1 - 2*q2*q2;
}
