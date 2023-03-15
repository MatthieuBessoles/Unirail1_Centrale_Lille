function [x_corr,y_corr] = marv_correc(x_k,y_k,x_k_minus,y_k_minus,T_e_marv, v_x,v_y)
%This function aims to proceed to the first step of the marvelmind data processing before passing them to the Kalman filter function.
var_x =(x_k-x_k_minus)/x_k_minus*100;
var_y =(y_k-y_k_minus)/y_k_minus*100;
if (abs(var_x) > 10) && (abs(var_y) > 10)
    x_corr=x_k_minus + T_e_marv*v_x;
    y_corr=y_k_minus + T_e_marv*v_y;
else
    x_corr = x_k;
    y_corr = y_k;
end
disp(x_corr);
disp(y_corr);
end