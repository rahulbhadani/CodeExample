function [time, detectedmessages, flag] = detect_signal(T, messageID, bus, signal_pos, signal_len)
    fprintf('\nInput: Message ID: %d,  Bus: %d, Signal Pos: %d, Signal Len: %d\n',  messageID, bus, signal_pos, signal_len);
    Msg_id = messageID;
    flag = 0;
    TM = T(T.MessageID == Msg_id, :);
    TM = TM(TM.Bus == bus,:);
    time = 0;
    detectedmessages = 0;
    if isempty(TM)
        fprintf("\nMessagage ID %d on Bus %d doesn't exist\n", messageID, bus);
        flag = -1;
        return;
    end
    
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