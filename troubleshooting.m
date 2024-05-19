clear; close all; clc;

data_exp = "(-)?[0-9](.)[0-9]{3}";
port = "/dev/cu.usbmodem1434201";


% data = "-0.001, 1.003, -0.003";
% eulers_str = regexpi(data, "(-)?[0-9](.)[0-9]{3}", 'match');
% eulers = double(eulers_str);


arduinoObj = serialport("/dev/cu.usbmodem1434201", 115200);
configureTerminator(arduinoObj,"CR/LF");
flush(arduinoObj);
arduinoObj.UserData = struct("Data",[],"Count",1);
configureCallback(arduinoObj,"terminator", @read_euler_data);


function read_euler_data(src, ~)
    data = readline(src);

    % eulers_str = regexpi(data, "(-)?[0-9](.)[0-9]{3}", 'match');
    % eulers = double(eulers_str);

    % src.UserData.Data(end+1) = eulers;
    src.UserData.Data(end+1) = str2double(data);
    src.UserData.Count = src.UserData.Count + 1;

    if src.UserData.Count>1000
        configureCallback(src, "off");
        plot(src.UserData.Data(2:end));
    end
end