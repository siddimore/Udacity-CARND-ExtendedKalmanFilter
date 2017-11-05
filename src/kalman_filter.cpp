#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

// Helper Method to CalculateUpdate
void KalmanFilter::CalculateUpdate(const VectorXd &z, const VectorXd &z_pred) {

  VectorXd y = z - z_pred;
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = P_ * Ht;
  MatrixXd K = PHt * Si;

  //new estimate
  x_ = x_ + (K * y);

  MatrixXd I = MatrixXd::Identity(x_.size(), x_.size());
  P_ = (I - K * H_) * P_;

}

// Helper Method to GetRadarMeasurements
MatrixXd GetRadarMeasurement(const VectorXd& x_state) {

  VectorXd z_radar;

  // check data is not a null_ptr
  if (&x_state != nullptr) {
    z_radar = VectorXd(3);
    //recover state parameters
    float px = x_state(0);
    float py = x_state(1);
    float vx = x_state(2);
    float vy = x_state(3);

    //TODO: YOUR CODE HERE

    //check division by zero
    z_radar(0) = sqrt(pow(px, 2) + pow(py, 2));

    if (sqrt(pow(px, 2) + pow(py, 2)) < .0001) {
      z_radar(2) = (px * vx + py * vy) / 0.0001;
    } else {

      // Check for px and py == 0 to avoid division by zero error
      if(px !=0 && py !=0) {
        z_radar(2) = (px * vx + py * vy) / sqrt(pow(px, 2) + pow(py, 2));
      }
    }

    z_radar(1) = atan2(py, px);

  }
  return z_radar;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */

  x_ = F_*x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;

}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */

  VectorXd z_pred = H_ * x_;
  this->CalculateUpdate(z, z_pred);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */

  VectorXd z_pred = GetRadarMeasurement(x_); //H_ * x_;
  this->CalculateUpdate(z, z_pred);
}


