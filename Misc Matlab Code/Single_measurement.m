clear all;
clc;
bit_rate = 115200

%check of serial ports available
available_port = serialportlist("available")
%Getting serial port ready
arduinoObj = serialport(available_port,bit_rate)
%No idea what it does
configureTerminator(arduinoObj,"CR/LF");
%clear previous data
flush(arduinoObj);
%Creating data stric - investigate that one more
arduinoObj.UserData = struct("Data",[],"Count",1)
%callback
configureCallback(arduinoObj,"terminator",@readSineWaveData);
