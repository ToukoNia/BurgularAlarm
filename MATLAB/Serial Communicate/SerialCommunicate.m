clear arduino
arduino = serialport("COM15",9600)
configureTerminator(arduino,"CR/LF")

armSystem="1";
while (1)
    if (readline(arduino)=="Check Login")
        in=input("Would you like to enter your username and password? (C to cancel): ","s");
        if (in~="C")
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

    if (readline(arduino)=="Check State")
end
    
clear arduino

