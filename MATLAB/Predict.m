% Funtion predict captures faces and predicts who that person is by
% printing to the screen. nameofs0x need to be changed accordingly, and x
% needs to be the correct number of people that can be recognised by the
% algorithm (from training stage), 

% need to manage updating names to the new
% users, adding new users easily, setting needed accuracy

%modifying to work with unknown amount of users
function userID = Predict(n,str,newnet,numberOfSubjects)

% Test a new Image
% use code below with giving path to your new image


count = zeros(numberOfSubjects,1); t=0;

%% Capturing faces using capturefacesfromvideo.m for prediction
delete(['photos\',str,'\*.jpg']);
capturefacesfromvideo(n,str);

delete(['croppedfacesTest\',str,'\*.jpg']);
ds1 = imageDatastore(['photos\',str],'IncludeSubfolders',true,'LabelSource','foldernames');
cropandsave(ds1,str,0);

%% Predicting the face
for i=1:n  %taking the n images
    try
        img = imread(['croppedfacesTest\',str,'\',int2str(i),'.jpg']);
        img = imresize(img,[227 227]);
    
        % can use [predict,score] = classify(newnet,img) here score says the percentage how confidence it is
        [predict,scores] = classify(newnet,img) %it will always match an image to a label 
        t=t+1;
        for j=1:numberOfSubjects
            str1 = ['s0',int2str(j)];
            if predict==str1&&max(scores)>0.45 %needs 0.5 for tara
                count(j) = count(j) + 1;
                %fprintf('The face detected is %s \n',nameofs01);
            end
        end
    catch
        break
    end
    
end

[M,I] = max(count);

if M<t*0.7;
    userID=-1;
else
    userID=I;
end

