% Function to train a NN to classify faces. Input parameter is the number
% of subjects. Output argument is the NN model (needed for the predict
% stage). 

function newnet = SimpleFaceRecognition(n)


%% looping through all subjects and cropping faces if found
% extract the subject photo and crop faces and saving it in to respective
% folders

% You can comment this out if you do not need to take photos for training

for i =1:n
    str = ['s0',int2str(i)];
    delete(['photos\',str,'\*.jpg']); %delete current photos from folder
    capturefacesfromvideo(300,str);
    delete(['croppedfaces\',str,'\*.jpg']);
    ds1 = imageDatastore(['photos\',str],'IncludeSubfolders',true,'LabelSource','foldernames');
    cropandsave(ds1,str,1);
end

%% Training a NN 
 im = imageDatastore('croppedfaces','IncludeSubfolders',true,'LabelSource','foldernames');
 % Resize the images to the input size of the net
 im.ReadFcn = @(loc)imresize(imread(loc),[227,227]);
 [Train ,Test] = splitEachLabel(im,0.8,'randomized');
 fc = fullyConnectedLayer(numel(categories(im.Labels)));
 net = alexnet;
 ly = net.Layers;
 ly(23) = fc;
 cl = classificationLayer;
 ly(25) = cl; 
 % options for training the net if your newnet performance is low decrease
 % the learning_rate
 learning_rate = 0.00001;
 opts = trainingOptions("rmsprop","InitialLearnRate",learning_rate,'MaxEpochs',5,'MiniBatchSize',64,'Plots','training-progress');
 [newnet,info] = trainNetwork(Train, ly, opts);
 
 [predict,scores] = classify(newnet,Test);
 names = Test.Labels;
 pred = (predict==names);
 s = size(pred);
 acc = sum(pred)/s(1);
 fprintf('The accuracy of the test set is %f %% \n',acc*100);

 %save("NNmodel.mat","newnet") % saves model to a .mat file if you wanted
 %to skip the training process

