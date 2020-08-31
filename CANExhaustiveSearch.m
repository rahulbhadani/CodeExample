function CANExhaustiveSearch(folder, csvfile, data2writefile, makeplot)

    % (C) Rahul Bhadani
    % Function for performing exhaustive search on CAN messages to search for interesting signals
    if nargin < 4
        makeplot = false;
    end
    % myCluster = parcluster('local');
    % myCluster.NumWorkers =8;  % 'Modified' property now TRUE
    % saveProfile(myCluster);    % 'local' profile now updated,
    %folder = "/home/ivory/CyverseData/JmscslgroupData/PandaData/2020_08_13";
    %file_name = "2020-08-13-13-26-45_2T3Y1RFV8KC014025_CAN_Messages.csv";
    file_name = csvfile;

    % data2writefile = "/home/ivory/VersionControl/CodeExample/ExhaustiveSearchMat";

    fid = fopen(folder  + "/" + file_name);
    data = textscan(fid,'%f %d %d %s %f','headerlines', 1, 'delimiter', ',');
    fclose(fid);
    
    correlationfile = data2writefile + "/"+"CorrCoeffSpeed.csv";
    if ~isfile(correlationfile)
        fileID = fopen(correlationfile,'w');
        fprintf(fileID,'MessageID,Bus,SignalPos,SignalLength,CorrCoeffWithSpeed\n');
        fclose(fileID);
    end


    n_col = length(data);
    len_cols = zeros(1, n_col);

    for i = 1:n_col
    len_cols(i) = length(data{i});
    end

    [val_min, ~] = min(len_cols);

    for i = 1:n_col
    if length(data{i}) > val_min
        data{i} = data{i}(1:end - ( length(data{i})  -val_min ));
    end
    end

     T = table(data{1}, data{2}, data{3}, data{4}, data{5});
     T.Properties.VariableNames = ["Time", "Bus", "MessageID", "Message", "MessageLength"];

     %MessageIDs = unique(T.MessageID);
     %MessageIDs =  MessageIDs';
     MessageIDs = [902];
     %Len = length(MessageIDs);

     ts_speed = get_speed(T);
     desired_frequency = 50; % 50Hz
     
     

     for id = 1:numel(MessageIDs)
         M_id = MessageIDs(id);
         M_id
         TM = T(T.MessageID == M_id, :);
         sig_length = [linspace(1, 16, 16), 32];

         folder = sprintf("%s/%d", data2writefile, M_id);
         if ~exist(folder, 'dir')
               mkdir(folder);
         end

         for bus = 0:2
            TB = TM(TM.Bus == bus,:);
            if isempty(TB)
                fprintf("\nMessagae with ID %d on Bus %d doesn't exist\n", M_id, bus);
                continue;
            end
             for sig_pos = 1:64

                for sig_len = sig_length

                    pngname = sprintf("%s/%d/DetectedSignal_MAT-Bus_%02d-SignalPos_%02d-SignalLen_%02d.png", data2writefile, M_id, bus, sig_pos, sig_len);

                      if makeplot == true
                          if isfile(pngname)
                            fprintf("\npngfile %s already exists, continuing\n", pngname);
                            continue;
                          end
                      end


                    [time, detectedval, flag] = detect_signal(TB, M_id, bus, sig_pos, sig_len);
                    if flag == -1
                        continue;
                    end

                    ts_msg =  timeseries(detectedval, time);

                    if 1/mean(diff(ts_msg.Time)) < desired_frequency
                        frequency = 1/mean(diff(ts_msg.Time)) ;
                    else
                        frequency = desired_frequency;
                    end
                    
                    ts_msg
                    frequency
                    % Synchronize the newly obtained timeseries with speed timeseries so as to calculate correlation coefficient
                     [ts_msg_resampled, ts_speed_resampled] = synchronize(ts_msg, ts_speed, 'Uniform','Interval', 1.0/frequency, 'InterpMethod','zoh');
                    ts_msg_resampled
                    ts_speed_resampled
                     % Calculate correlation Coefficient
                     cf= corrcoef(ts_msg_resampled.Data(1,:), ts_speed_resampled.Data(1,:));
                     cf
                     cf2 = cf(2);

                    fileID = fopen(correlationfile,'a');
                    fprintf(fileID,'%d, %d,%d,%d,%f\n', M_id, bus, sig_pos, sig_len, cf2);
                    fclose(fileID);

                    if makeplot == true
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
                        legend({sprintf("Message ID: %d, Bus: %d, Signal Pos: %d, Signal Len: %d", M_id, bus, sig_pos, sig_len)});
                        set(gcf, 'InvertHardCopy', 'off'); 
                        saveas(gcf, pngname);
                        close(f);
                    end
                end
             end
         end
     end
end