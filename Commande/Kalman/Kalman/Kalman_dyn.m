function [x_k_hat,P_k] = Kalman_dyn(x_k_hat_b,P_k_b,H,A,B,u_k,Q,z_k)
%This function aims to calculate a reliable state value based on the
%measurements from our two sensors and the dynamic prediction.
x_k_hat_minus = A*x_k_hat_b+B*u_k;
P_k_minus = A*P_k_b*A'+Q;
K_k = P_k_minus*H'*(H*P_k_minus*H'+R)^(-1);
x_k_hat = x_k_hat_minus + K_k*(z_k-H*x_k_hat_minus);
P_k = (I - K_k*H)*P_k_minus;
end