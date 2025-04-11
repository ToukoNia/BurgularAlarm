clear arduino
arduino = serialport("COM15",9600)
configureTerminator(arduino,"CR/LF")


while (1) 
    if (readline(arduino)=="User Logged in") %need to add something like activating in 30 seconds, and then activate
        armSystem = input("Press A to arm the system: ","s");
        writeline(arduino,armSystem);
    end
    if (readline(arduino)=="Admin Logged in")
        systemChoice=input("[A]rm the system, [T]est the system, [U]pdate credentials: ","s");
        writeline(arduino,systemChoice);
    end
    if (readline(arduino)=="Check Login")
        username=input("Username: ","s");
        password=input("Password: ","s");
        writeline(arduino,"1");
        while(1)
            if (readline(arduino)=="Check Username")
                writeline(arduino,username);
                break
            end
        end
        while(1)
             if (readline(arduino)=="Check Password")
                writeline(arduino,password);
                break
             end
        end
    end
    break
end


    
clear arduino

