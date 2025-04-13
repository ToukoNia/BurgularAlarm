clear arduino
arduino = serialport("COM15",9600)
configureTerminator(arduino,"CR/LF")


while (1) 
    message=readline(arduino);
    if (message=="User Logged in") %need to add something like activating in 30 seconds, and then activate
        armSystem = input("Press A to arm the system, or L to logout: ","s");
        writeline(arduino,armSystem);
    elseif (message=="Admin Logged in")
        systemChoice=input("[A]rm the system, [T]est the system, [U]pdate credentials or [L]ogout: ","s");
        writeline(arduino,systemChoice);
        if (systemChoice=="U")
            systemChoice=input("[A]dd or [R]emove a user: ","s");
            while(1)
                if (readline(arduino)=="Add or Remove")
                    writeline(arduino,systemChoice);
                    break
                end
            end
            username=input("Username: ","s");

            while(1)
              if (readline(arduino)=="Get Username")
                    writeline(arduino,username);
                    break
              end
            end
            if systemChoice=="A"
                systemChoice=input("Type 1 if admin: ","s");
                while(1)
                    if (readline(arduino)=="Get Admin")
                        writeline(arduino,systemChoice);
                        break
                    end
                end
            else
                disp(readline(arduino));
            end
        end
    elseif (message=="Check Login")
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
end


    
clear arduino

