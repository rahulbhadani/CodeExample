%% (C) Rahul Kumar Bhadani
% DRAWCUBE Function to draw a cube
% DRAWCUBE(D,SIZE,XTRANSLATION,YTRANSLATION,ZTRANSLATION) draws a cube
% specified with vertex at(D,D,D) and diagonally opposite vertex at
% (D-SIZE,D-SIZE,D-SIZE). Each cube will have dimension specified by SIZE.
% XTRANSLATION will specify the translation of cube specified by D and SIZE
% in x-direction. YTRANSLATION will specify the translation of cube 
% specified by D and SIZE in y-direction. ZTRANSLATION will specify the 
% translation of cube specified by D and SIZE in z-direction. 

function  drawCube(d, size, xtranslation, ytranslation, ztranslation)
    [X, Y] = meshgrid(d:size:d+size);
    Z = zeros(length(X))+ d;
    surf(X+xtranslation,Y+ytranslation,Z+size+ztranslation,'Facecolor','Blue','EdgeColor','Blue');

    hold on;
   drawnow;
    surf(X+xtranslation,Y+ytranslation,Z+ztranslation,'Facecolor','Yellow','EdgeColor','Blue');

   drawnow;
    
    surf(Z+xtranslation,X+ytranslation,Y+ztranslation,'Facecolor','Blue','EdgeColor','Black');

   drawnow;
    surf(Z+size+xtranslation,X+ytranslation,Y+ztranslation,'Facecolor','Red','EdgeColor','Green');

   drawnow;
    
    surf(X+xtranslation,Z+ytranslation,Y+ztranslation,'Facecolor','Green','EdgeColor','Cyan');

   drawnow;
    surf(X+xtranslation,Z+size+ytranslation,Y+ztranslation,'Facecolor','Blue','EdgeColor','Yellow');

   drawnow;
    xlabel('X');
    ylabel('Y');
    ylabel('Z');
end