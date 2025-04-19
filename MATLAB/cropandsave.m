function  cropandsave(im,str,t)
j = 1;
T = countEachLabel(im);
n = T(1,2).Variables;
for i = 1:n
    i1 = readimage(im,i);
    [img,face] = cropface(i1);
    if (face==1) && (t==1) %folder for training data
        imwrite(img,['croppedfaces\', str,'\',int2str(j), '.jpg']);
        j = j+1;
    elseif (face==1) && (t==0) %store test photos in a different folder
        imwrite(img,['croppedfacesTest\', str,'\',int2str(j), '.jpg']);
        j = j+1;
    end
end
