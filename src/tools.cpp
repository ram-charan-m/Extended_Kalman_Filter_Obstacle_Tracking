#include "tools.h"
#include <iostream>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;
using std::endl;
using std::cout;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  VectorXd rmse(4);
  rmse << 0,0,0,0;

  // check the validity of the following inputs:
  if ((estimations.size()==0) || (estimations.size()!=ground_truth.size())) {
  // the estimation vector size should not be zero
  // the estimation vector size should equal ground truth vector size
  cout<<"INVALID ESTIMATES."<<endl;
  }

  // Accumulating Residuals
  for (int i=0; i < (int)estimations.size(); ++i) {
    VectorXd residuals = estimations[i]-ground_truth[i];
    residuals = residuals.array()*residuals.array();
    rmse += residuals;
  }

  // Mean
  rmse = rmse/estimations.size();

  // Squared root
  rmse = rmse.array().sqrt();

  return rmse;    
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {

  MatrixXd Hj(3,4);
  // recover state parameters
  float px = x_state(0);
  float py = x_state(1);
  float vx = x_state(2);
  float vy = x_state(3);

  // check division by zero
  
  if((px && py)!=0){
        // TODO: YOUR CODE HERE 
        float px_2 = px*px;
        float py_2 = py*py;
//         if ((px_2 + py_2) < 0.0001){ // Avoiding NaNs 
//           px += 0.001;
//           py += 0.001;
//           px_2 = px*px;
//           py_2 = py*py;
//         }
        float Hj00 = px/(pow((px_2 + py_2),0.5));
        float Hj01 = py/(pow((px_2 + py_2),0.5));
        float Hj10 = -py/(px_2 + py_2);
        float Hj11 = px/(px_2 + py_2);
        float Hj20 = (vx*py_2 - vy*px*py)/(pow((px_2 + py_2),1.5));
        float Hj21 = (vy*px_2 - vx*px*py)/(pow((px_2 + py_2),0.5));
        
        // compute the Jacobian matrix
        Hj << Hj00, Hj01, 0, 0,
            Hj10, Hj11, 0, 0,
            Hj20, Hj21, Hj00, Hj01;
  }
  else{
      cout<<"ERROR in Jacobian calculation. Detected NaNs"<<endl;
  }
  return Hj;
}