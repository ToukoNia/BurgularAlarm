clear arduino
arduino = serialport("COM16",9600); %sets up serial object
configureTerminator(arduino,"CR/LF"); %sets the line terminators to the correct type to ensure successful reading
users=["Nia","Mumin"]
while (1) 
    message=readline(arduino);  %see what state the arduino is in, is neccessary to know when logging in, logging out, and if armed/disarmed
    if (message=="User Logged in") %need to add something like activating in 30 seconds, and then activate
        armSystem = input("Press A to arm the system, or L to logout: ","s"); 
        writeline(arduino,armSystem); %tells the arduino the selected option
    elseif (message=="Admin Logged in") %Admin pannel
        systemChoice=input("[A]rm the system, [T]est the system, [U]pdate credentials or [L]ogout: ","s");
        writeline(arduino,systemChoice);
        if (systemChoice=="U")
            systemChoice=input("[A]dd or [R]emove a user: ","s"); %gets what to do, and writes it to the arduino
            communicate(arduino,"Add or Remove",systemChoice);
            username=input("Username: ","s");
            users(end+1)=username; %adds the new username to the array
            communicate(arduino,"Get Username",username); %communicates the neccessary information to identify what is being selected
            if systemChoice=="A"
                userCount=length(users);
                newnet=SimpleFaceRecognition(userCount,userCount); %adds a new face at the registered value
                systemChoice=input("Type 1 if admin: ","s");
                communicate(arduino,"Get Admin",systemChoice);  %NOTE: this might time out, idk what our fix is if that happens, mayb place it after this bc then were looping the arduino instead lmao
            else
                index=find(users=="username"); %finds the index
                if index %if in array
                    users(index)=[]; %removes the username
                    removeUserFromFacialID(index,userCount); %removes the user
                    newnet=SimpleFaceRecognition(0,0); %reruns the facial recognition with the change
                end
            end
            disp(readline(arduino));    %display success/error message to user for adding/removing, after the arduino has done the computation
        end
    elseif (message=="Check Login") %calls to see if there is a login
        password=input("Password: ","s");
        userID=Predict(50,'s01Test',newnet,length(users));
        if userID>0
            username=users(userID);
        else 
            username="Failed";
        end
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
   for i=userNo:numberOfUsers-1 %did the logic off vibes so could be wrong lmao
       str = ['s0',int2str(i)];
       str1 = ['s0',int2str(i+1)];
       movefile(['photos\',str1,'\*.jpg'],['photos\',str]) %source,destination shifts them one folder backwards
   end
end
