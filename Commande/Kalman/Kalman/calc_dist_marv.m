function [dist_marv] = calc_dist_marv(dist_parc_marv, x_k,y_k,x_k_minus,y_k_minus)
%This function aims to proceed to the second step, meaning the calculation
%of the distance reached given by the marvelmind. Attention cependant au
%répère marvelmind
d_k =sqrt((x_k-x_k_minus)^2+(y_k-y_k_minus)^2);
disp(d_k)
dist_marv = dist_parc_marv + d_k;
end