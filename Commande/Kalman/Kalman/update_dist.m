function [new_canton_marv,new_dist_marv] = update_dist(canton_now, pos_debut_canton,radius_update,x_k,y_k)
%update_dist uses the marvelmind coordinates of the train to update the
%canton the train is on, and the traveled distance to zero once the train
%as entered the new canton, so that one will be able to compare the new
%traveled distance with the odometric one. 
%INPUT : canton_now is the index in the train_i's canton_list of the canton the train is on atm 
dist_t_b = sqrt((x_k-pos_debut_canton(1))^2+(y_k-pos_debut_canton(2))^2); %distance between the train and the balise
if dist_t_b < radius_update
    new_dist_marv = 0;
    new_canton_marv = canton_now + 1; % il faudra faire une gestion des indices dans la liste
end
end