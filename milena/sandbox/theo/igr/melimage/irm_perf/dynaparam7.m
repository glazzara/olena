function [reh,tmax]=DYNAPARAM7(image,roi1,roi2)
% Création d'images paramétriques du rehaussement et du temps ou le maximum
% du rehaussement est atteint, pour DCE-MRI
%alw le 15 octobre 2007 avec interaction fructueuse G Begin
%tracer préalablement la ROI1 (bruit) pour SEUILLAGE 
%et FACULTATIF la ROI2 pour région de travail

%PARAM7 version 24 fevrier 2009 (le protocole est enfin figé)

%DONNER pour le graphique temps echantillonnage entre deux images =acqui
%calcul sur les dim3 images
%Choix du seuil f; quatre  est une bonne valeur
%donner aussi ini le nombre d images qui sont la ligne de base

%NORMALISATION
%AUC est l intégrale de l augmentation de signal
%AUCnorm : division AUC voxel à voxel par "imaini"=intégrale de (REH-1) sur la série

acqui=3  % durée acqui et temps interimages; en secondes
fseuil=4 % ratio entre signaux trop faibles, éliminés, et bruit de fond mesuré sur l image
ini=9    %nombre images ligne de base
ini2=20  % a la fin de la montée vasculaire, à la dixieme image post injection: lissage des images par trois

%calcul signal initialmoyen : 8 images de 2 à 9 moyennées=ligne de base
imageini=0.125*(image(:,:,2)+image(:,:,3)+image(:,:,4)+image(:,:,5)+image(:,:,6)+image(:,:,7)+image(:,:,8)+image(:,:,9));

figure(1)
imagesc(abs(imageini(:,:)));
title('image base');

dim1=size(image,1);
dim2=size(image,2);
dim3=size(image,3);

  %calcul auc aire sous la courbe
  for k=1:dim3
      imasoustraite(:,:,k)=image(:,:,k)-imageini;
  end;
  
%Mesure bruit de fond pour seuiller
%calculé sur la première image
nbrpix1=sum(sum(roi1));
 datasli=image(:,:,1);
 prodsignal1=datasli.*roi1;
%moyenne
moysignal1=sum(sum(abs(prodsignal1)))/nbrpix1;
%ecart type
 som1=0;
kk=0;
for l=1:size(prodsignal1,1)
    for k=1:size(prodsignal1,2)
        if(roi1(l,k)~=0)
            som1=som1+(abs(datasli(l,k))-moysignal1)^2;
        end;
    end;
end;
ectys=sqrt(som1/(nbrpix1-1));
seuil=fseuil*ectys;

%on calcule le masque
for i=1:dim1
    for j=1:dim2
        if(abs(image(i,j,1))>seuil)
        masque(i,j)=1.0;
        else
        masque(i,j)=0.0;
        end;
    end;
end;
% si on a choisi une région avec roi2
if (nargin>2)
    masque=masque.*roi2;
end;

%on applique le masque sur image et imasoustraite

for k=2:dim3
    for i=1:dim1
        for j=1:dim2
            if(masque(i,j)==0)
            image(i,j,k)=0;
            imasoustraite(i,j,k)=0;
            end;
        end;
    end;
end;

%on regarde si le seuillage et le masquage sont OK
figure(2)
imagesc(abs(image(:,:,2)));
title('image seuillée');

% on cherche les maxi (intensité c et temps T) des courbes signal(temps)
%on masque toute la série d'images

%Essai de lissage à parir de fin de montée vaculaire ini2 pour ameliorer
%les seuillages a 10 50 90        NON EVALUE RIGOUREUSEMENT
for k=ini2:dim3-1
image(:,:,k) = 0.5*image(:,:,k)+0.25*image(:,:,k-1)+0.25*image(:,:,k+1);
end;

kk=0;
for i=1:dim1
       for j=1:dim2
           if(masque(i,j)==0)
           image(i,j,:)=0;
           c(i,j)=0;
            T(i,j)=0;
            kk=kk+1;
        end;
    end;
end;
        [c,T]=max(image,[ ],3);
     
% ou c est la valeur du max et T son index, le long de la dimension 3
%kk est le nombre de points masques
kk

 %calcul AUC à partir de imagesoustraite
 AUC=zeros(dim1,dim2); 
 for i=1:dim1
       for j=1:dim2
            for k=1:dim3
         AUC(i,j)=AUC(i,j)+imasoustraite(i,j,k); 
            end;
        end;
    end;
    
% Conversion du temps du pic en secondes à partir fin période de base
tmax=acqui*(T-ini);

%calcul des temps 10 et 90 et de la pente correspondante
for i=1:dim1
       for j=1:dim2
           [C10(i,j),idx_10(i,j)]=max(image(i,j,:)>=0.1*c(i,j));
           [C90(i,j),idx_90(i,j)]=max(image(i,j,:)>=0.9*c(i,j));
            [C50(i,j),idx_50(i,j)]=max(image(i,j,:)>=0.5*c(i,j));
      if(idx_90(i,j)-idx_10(i,j)==0)
      masque2(i,j)=0;
      else
      masque2(i,j)=1;
      end;
       end;
end;         

 %calcul MTT
 
 %QUAND le signal redescent significativement après le maximum
 %elimination des voxels où le signal est encore 90% du maximum en fin
 %d'acquisition
 kk2=0;
for i=1:dim1
    for j=1:dim2
          
               if (image(i,j,dim3)>=0.9*c(i,j))
                  
                   masque3(i,j)=0;
                   kk2=kk2+1;
               else
           masque3(i,j)=1;
               end;
 end;
 end;
 
 %pour les voxels non masqués on inverse t et on cherche le max 50% a partir de la fin
 for i=1:dim1
       for j=1:dim2
          for k=1:dim3
   imageinv(i,j,k)=masque3(i,j)*image(i,j,dim3+1-k);
   end;
   end; 
 end;  
 
 for i=1:dim1
       for j=1:dim2  
            if (masque3(i,j)==0)
                idx_51(i,j)=0;
            C51(i,j)=0;
            else
           [C51(i,j),idx_51(i,j)]=max(imageinv(i,j,:)>=0.8*c(i,j));
            end;
       end;
end;         
       
 kk2
 
 %Calcul MTT  defini comme largeur a mi hauteur de la courbe signal(temps)
 %soit t50 montant et t50 descendant
  for i=1:dim1
       for j=1:dim2
           if(masque(i,j)*masque2(i,j)*masque3(i,j)==0)
               MTT(i,j)=0;
           else
             MTT(i,j)=acqui*(dim3-idx_51(i,j)-idx_50(i,j));
           end;
       end;
end;
 %calcul de la pente 10/90 (avec delta signal=80%du maximum c(i,j) et 
 %delta temps =t90 -t10
 for i=1:dim1
       for j=1:dim2
           if(masque(i,j)*masque2(i,j)==0)
               pente(i,j)=0;
           else
             pente(i,j)=(0.8*c(i,j))/(acqui*(idx_90(i,j)-idx_10(i,j)));
           end;
       end;
end;

  %calcul pente normalisée rehaussement normalisé et AUC normalisée
 for i=1:dim1
       for j=1:dim2
           if(masque(i,j)==0)
            pentenorm(i,j)=0;
            reh(i,j)=0;
            AUCnorm(i,j)=0;
           else
       pentenorm(i,j)=pente(i,j)/imageini(i,j);   
         reh(i,j)=c(i,j)/imageini(i,j); 
         AUCnorm(i,j)=AUC(i,j)/imageini(i,j);
  end;
    end;
end; 
 

figure (3)
imagesc(AUCnorm);
title('image aire normalisée sous la courbe');
% l'aire sous la courbe est négative là où il y a eu du bougé (bord tube)


figure (4)
imagesc(pentenorm);
title('pente normalisée10-90');

figure(5)
imagesc(reh);
title('image rehaussement relatif');
caxis([0 5]);
%au dela de 5 il faut réviser la dose d'agent de contraste car surdosage

figure(6)
imagesc(MTT);
title('image MTT');

figure(9)
imagesc(idx_51);
title('idx_51');

figure(10)
imagesc(C51);

figure(7)
imagesc(tmax);
title('image Tmaximum');

figure(8)
subplot(2,2,1);
imagesc(pentenorm);
title('pente normalisée10-90');
subplot(2,2,2)
imagesc(reh);
title('image rehaussement relatif');
caxis([0 3]);
subplot(2,2,3)
imagesc(tmax);
title('image Tmaximum');

% COURBE S(t)sur pixel choisi graphiquement
%Permet de voir la courbe de rehaussement d'un voxel pointé 
%SI LE VOXEL EST AU BORD DU TUBE TÉMOIN: UN DÉTECTEUR DE MOUVEMENTS
% coordonx=1:size(image,3);
x=size(image,1)/2;
y=size(image,2)/2;
while((x<size(image,1))&&(y<size(image,2)))
% la saisie du point de coordonnées x,y se fait sur la figure ouverte en
% dernier
% pour sortir: cliquer dans le gris...
% pour affichage interactif coordonnées pixel, mais en fait pas utile
h=figure(8)
pixval;
%saisie coordonnées du pixel pour lequel on veut afficher la courbe de
%décroissance et le fit
[x,y]=ginput(1);
% on ajuste a l'entier supérieur pour trouver les "vraies" coordonnées
% en plus il faut inverser entre xy et ij...
coordx=ceil(y);
coordy=ceil(x);
% test sortie du programme
if((coordx>size(image,1)) | (coordy>size(image,2)))
    break;
end;
fprintf(1,'\n%d\t%d\t%f\t%f\n',coordx,coordy,reh(coordx,coordy),image(coordx,coordy,1) );
titi=(abs(image(coordx,coordy,:)));
tata=squeeze(titi);
subplot(2,2,4)
plot(tata);
end;

