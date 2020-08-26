% (C) Rahul Bhadani
clc;
% myCluster = parcluster('local');
% myCluster.NumWorkers =8;  % 'Modified' property now TRUE
% saveProfile(myCluster);    % 'local' profile now updated,

clear all;
folder = "/home/ivory/CyverseData/JmscslgroupData/PandaData/2020_08_13/";
file_name = "2020-08-13-13-26-45_2T3Y1RFV8KC014025_CAN_Messages.csv";
%df = readtable(folder  + file_name,'TreatAsEmpty',{'.','NA'});

fid = fopen(folder  + file_name);
data = textscan(fid,'%f %d %d %s %f','headerlines', 1, 'delimiter', ',');
fclose(fid);

data{1} = data{1}(1:end-1);
data{2} = data{2}(1:end-1);
data{3} = data{3}(1:end-1);
data{4} = data{4}(1:end-1);

 T = table(data{1}, data{2}, data{3}, data{4}, data{5});
 T.Properties.VariableNames = ["Time", "Bus", "MessageID", "Message", "MessageLength"];

 MessageIDs = unique(T.MessageID);
 MessageIDs =  MessageIDs';
 Len = length(MessageIDs);
 parfor id = 1:Len
     M_id = MessageIDs(id);
     sig_length = [linspace(1, 16, 16), 32];

     folder = sprintf("/home/ivory/VersionControl/CodeExample/ExhaustiveSearchMat/%d", M_id);
     if ~exist(folder, 'dir')
           mkdir(folder);
     end

     for sig_pos = 1:64

         for sig_len = sig_length
            
             pngname = sprintf("/home/ivory/VersionControl/CodeExample/ExhaustiveSearchMat/%d/DetectedSignal_MAT-SignalPos_%d-SignalLen_%d.png", M_id, sig_pos, sig_len);
            
              if isfile(pngname)
                fprintf("\npngfile %s already exists, continuing\n", pngname);
                continue;
              end
            
             [time, detectedval, flag] = detect_signal(T, M_id, sig_pos, sig_len);
            if flag == -1
                continue;
            end

            f = figure('visible', 'off');
            set(f,'Units','Inches');
            f.Position = [0.9167 2.1146 18.1667 7.1458];
            scatter(time, detectedval, 5 ,'MarkerEdgeColor',[0 .5 .5],...
                  'MarkerFaceColor',[0 .7 .7]);
            
            title(sprintf("Exhaustive Search for Signal Detection, Messqge ID %d", M_id), 'FontSize', 10, 'Interpreter','none');
            grid on;
            grid minor;
            ax = gca;
            ax.GridColor = [130, 130, 130]/255;
            set(gca,'Color', [235, 235, 235]/255);
            set(gca,'FontSize',10);
            set(gca,'XColor', [130, 130, 130]/255,'YColor',  [130, 130, 130]/255,'TickDir','out');
            xlabel('Time (s)', 'Color', 'k');
            ylabel('[Unit]','Color', 'k');
            ylim([min(detectedval)*0.70 - 15.0, max(detectedval)*1.30 + 15.0]);
            legend({sprintf("Message ID: %d, Signal Pos: %d, Signal Len: %d", M_id, sig_pos, sig_len)});
            set(gcf, 'InvertHardCopy', 'off'); 
            saveas(gcf, pngname);
            close(f);
         end
     end
   
 end
 
function [time, detectedmessages, flag] = detect_signal(T, messageID, signal_pos, signal_len)
    fprintf('\nInput: Message ID: %d, Signal Pos: %d, Signal Len: %d\n',  messageID, signal_pos, signal_len);
    Msg_id = messageID;
    flag = 0;
    TM = T(T.MessageID == Msg_id, :);
    MLen = unique(TM.MessageLength);
    
    time = TM.Time;
    TMsize = size(TM);
    detectedmessages = zeros(1, TMsize(1));
    binarystr = strings(1, TMsize(1));
    
    if length(MLen) == 1
        fprintf("Message Length: %d\n", MLen);
        if (signal_pos > MLen*8) 
            fprintf("Specified Signal Position %d exceeded the message length boundary\n", signal_pos);
            flag = -1;
            return;
        end
        if (signal_len > MLen*8)
            fprintf("Specified Signal length %d exceeded the toal message length\n", signal_len);
            flag = -1;
            return;
        end
        if (signal_pos + signal_len > MLen*8)
            fprintf("Specified Signal length %d exceeded the message length boundary\n", signal_len);
            flag = -1;
            return;
        end
        
         for i = 1:TMsize(1)
            binarystr(i)  = hex2bin(TM.Message{i});
        end
        
    else
        fprintf("Variable Message Lengths: %d\n", MLen);
        for i = 1:TMsize(1)
            MLen = TM.MessageLength(i);
            if (signal_pos > MLen*8) 
                fprintf("Specified Signal Position %d exceeded the message length boundary\n", signal_pos);
                flag = -1;
                return;
            end
            if (signal_len > MLen*8)
                fprintf("Specified Signal length %d exceeded the toal message length\n", signal_len);
                flag = -1;
                return;
            end
            if (signal_pos + signal_len > MLen*8)
                fprintf("Specified Signal length %d exceeded the message length boundary\n", signal_len);
                flag = -1;
                return;
            end
            binarystr(i)  = hex2bin(TM.Message{i});
        end
        
    end
    

    for i = 1:TMsize(1)
        bin_val = char(binarystr(i));
        slice = bin_val(signal_pos:signal_pos+signal_len-1);
        detectedmessages(i) = bin2dec(slice);
    end
    
end
 
 

function [binstr] = hex2bin(hexstr)
    binstr ="";
    hexchar = char(hexstr);
    for j = 1:strlength(hexchar)
        h = dec2bin(hex2dec(hexchar(j)), 4);
        binstr = binstr + h;
    end
end