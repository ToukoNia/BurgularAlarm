clear arduino
arduino = serialport("COM16",9600); %sets up serial object
configureTerminator(arduino,"CR/LF"); %sets the line terminators to the correct type to ensure successful reading
users=["Nia","Mumin","Callum","David","Aly"];
%newnet=SimpleFaceRecognition(1,2); Sets up the network
%{
systemChoice = input("Do you want to load the system or create a new system");
filename = 'arduino_data.txt';
fid = fopen(filename, ['r']);
if fid == -1
    error('Failed to open file for reading.');
end
%}

FileID = fopen('credentials.txt','r');
password = fgetl(FileID);
attempts = fgetl(FileID);
fclose(FileID);

communicate(arduino,"Password",password);
communicate(arduino,"Attempts",attempts);

SendFile(arduino, 'users.txt', "Users End");

SendFile(arduino, 'sensors.txt', "Sensors End");

SendFile(arduino, 'locks.txt', "Locks End");
message=readline(arduino)
while(message~="Credentials End")
    message=readline(arduino)
end

while (1) 
    message=readline(arduino);  %see what state the arduino is in, is neccessary to know when logging in, logging out, and if armed/disarmed
    if (message=="User Logged in") %need to add something like activating in 30 seconds, and then activate
        armSystem = input("Press A to arm the system, or L to logout: ","s"); 
        writeline(arduino,armSystem); %tells the arduino the selected option
    elseif (message=="Admin Logged in") %Admin pannel
        systemChoice=input("[A]rm the system, [T]est the system, [U]pdate users, update [C]redentials or [L]ogout: ","s");
        writeline(arduino,systemChoice);
        if (systemChoice=="U")
            systemChoice=input("[A]dd or [R]emove a user: ","s"); %gets what to do, and writes it to the arduino
            communicate(arduino,"Add or Remove",systemChoice);
            username=input("Username: ","s");
            communicate(arduino,"Get Username",username); %communicates the neccessary information to identify what is being selected
            if systemChoice=="A"
                users(end+1)=username; %adds the new username to the array
                userCount=length(users);
                newnet=SimpleFaceRecognition(userCount,userCount); %adds a new face at the registered value
                systemChoice=input("Type 1 if admin: ","s");
                communicate(arduino,"Get Admin",systemChoice);  %NOTE: this might time out, idk what our fix is if that happens, mayb place it after this bc then were looping the arduino instead lmao
            else
                index=find(users==username); %finds the index
                if index %if in array
                    users(index)=[]; %removes the username
                    removeUserFromFacialID(index,length(users)); %removes the user
                    newnet=SimpleFaceRecognition(0,0); %reruns the facial recognition with the change
                end
            end
            disp(readline(arduino));    %display success/error message to user for adding/removing, after the arduino has done the computation
        elseif systemChoice=="C"    %updates the credentials
            password=input("Enter the new password ","s");
            attempts=input("How many attempts: ","s");      %make this 0 if no input
            communicate(arduino,"New Password",password);   %make this 0 if no password
            communicate(arduino,"Attempt Change",attempts);
        elseif systemChoice=="S"
             save(newnet);
             saveInformation(arduino,"Credentials Start", "Credentials End","credentials.txt");
             saveInformation(arduino,"User List Start", "User List End","users.txt");
             saveInformation(arduino,"Sensor List Start", "Sensor List End","sensors.txt");
             saveInformation(arduino,"Lock List Start", "Lock List End","locks.txt");
        end
    elseif (message=="Check Login") %calls to see if there is a login
        password=input("Password: ","s");
        userID=Predict(50,'s01Test',newnet,length(users));
        if userID>0
            username=users(userID);
        else 
            username="Failed Login Attempt";
        end
        username = "Nia" %REMOVE LATER
        writeline(arduino,"1"); %if there is a login write 1 to the arduino
        communicate(arduino,"Check Username",username) %then communicate to the arduino the username and password (NOTE: the username step will be changed when i add facial fully)
        communicate(arduino,"Check Password",password)
    end
end

function communicate(arduino,prompt,info)   %communicates w the arduino in a way that avoids errors
    while(1)
        if (readline(arduino)==prompt)
            writeline(arduino,info);
            break
        end
    end
end

function removeUserFromFacialID(userNo,numberOfUsers)   %removes users from the directories
   str = ['s0',int2str(userNo)];
   delete(['photos\',str,'\*.jpg']);
   delete(['croppedfaces\',str,'\*.jpg']);
   for i=userNo:numberOfUsers %did the logic off vibes so could be wrong lmao
       str = ['s0',int2str(i)];
       str1 = ['s0',int2str(i+1)];
       movefile(['photos\',str1,'\*.jpg'],['photos\',str]) %source,destination shifts them one folder backwards
       movefile(['croppedfaces\',str1,'\*.jpg'],['photos\',str]) %source,destination shifts them one folder backwards
   end
end

function [newnet,users]=SetupBasicSystem()  %need a way tp add to the arduino too
    users=[];
    userCount=input("How many users? ");
    for i=1:userCount
        users(i)=input("Username: ","s");
    end
    for i=(length(users)+1:12)
       str = ['s0',int2str(i)];
       delete(['photos\',str,'\*.jpg']);
       delete(['croppedfaces\',str,'\*.jpg']);
    end

    newnet=SimpleFaceRecognition(1,userCount);
end 



%before this set up an if statement reading in a line to check if it
%matches the start prompt, if so run this function to get all values
function array=readUntilStop(arduino,endMessage)
    array = string([]);
    message=readline(arduino);
    while message~=endMessage
        array(end+1)=message;
        message=readline(arduino);
    end
end

function saveInformation(arduino,prompt,endPrompt,filename)
    while (readline(arduino)~=prompt)
    end
    writeline(arduino,"1");
    arr=readUntilStop(arduino,endPrompt);
    fid=fopen(filename,"w");
    if fid == -1
        error('Failed to open file for writing.');
    end
    saveToFile(fid,arr);
    fclose(fid);
end

function saveToFile(fid,array)
    for i=1:length(array)
        fprintf(fid, '%s\n', array(i));
    end
end

function SendFile(arduino, FileName, EndPrompt)
    FileID = fopen(FileName);
    line = fgetl(FileID);
    while ischar(line)
        communicate(arduino,"NEXT",string(line));
        line = fgetl(FileID);
    end
    communicate(arduino,"NEXT",EndPrompt);
    fclose(FileID);
end

