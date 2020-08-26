function [time, detectedmessages, flag] = detect_signal(T, messageID, signal_pos, signal_len)
    fprintf('\nInput: Message ID: %d, Signal Pos: %d, Signal Len: %d\n',  messageID, signal_pos, signal_len);
    Msg_id = messageID;
    flag = 0;
    TM = T(T.MessageID == Msg_id, :);
    MLen = unique(TM.MessageLength);
    fprintf("Message Length: %d\n", MLen);
    time = TM.Time;
   TMsize = size(TM);
    detectedmessages = zeros(1, TMsize(1));
    binarystr = strings(1, TMsize(1));
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
    

    for i = 1:TMsize(1)
        bin_val = char(binarystr(i));
        slice = bin_val(signal_pos:signal_pos+signal_len-1);
        detectedmessages(i) = bin2dec(slice);
    end
    
end