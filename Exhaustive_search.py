import numpy as np
import matplotlib
#matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from celluloid import Camera
import os

import pandas as pd
import numpy as np
import scipy
import matplotlib.pyplot as plt
from strym import strymread
import matplotlib.animation as animation

#folder = "/home/ivory/CyverseData/JmscslgroupData/PandaData/2020_05_12/"
folder = "/home/ivory/CyverseData/JmscslgroupData/PandaData/2020_08_13/"
#file_name = "2020-05-12-13-25-00_2T3Y1RFV8KC014025_CAN_Messages.csv"
file_name = "2020-08-13-13-26-45_2T3Y1RFV8KC014025_CAN_Messages.csv"
df = pd.read_csv(folder + file_name)
df.dropna(inplace=True)

def detect_signal(df, messageID = 180, signal_pos = 40, signal_len = 16):
    print("-----------------------------------")
    print("Detected Signals for inputs: messageID:{}, signal_pos:{}, signal_len:{}".format(messageID, signal_pos,signal_len))
    mID = df[df['MessageID'] == messageID]
    mLen = mID['MessageLength'].unique()[0]

    if (signal_pos > int(mLen*8) - 1):
        print("Specified Signal Position exceeded the message length boundary")
        return None

    if (signal_len > int(mLen*8)):
        print("Specified Signal length exceeded the toal message length")
        return None

    if (signal_pos + signal_len > int(mLen*8)):
        print("Specified Signal length exceeded the message length boundary")
        return None


    m_list = []
    time_list = []
    for index, row in mID.iterrows():
        Message = row['Message']
        MessageLen = row['MessageLength']
        bits = format(int('0x'+Message, 16), '0{}b'.format(int(MessageLen)*8))
        message_of_interest = bits[signal_pos:signal_pos+signal_len]
        message_of_interest = int(message_of_interest, 2)


        time_list.append(row['Time'])
        m_list.append(message_of_interest)


    dfnew = pd.DataFrame(columns=['Time', 'Detected_Values'])
    dfnew['Time'] = time_list
    dfnew['Detected_Values'] = m_list
    return dfnew

# msg_unique = [186, 291, 296, 304, 352, 416, 417, 418, 419, 426, 436, 437, 438, 440, 441, 442, 443, 464,
#              544, 562, 565, 578, 579, 580, 581, 582, 583, 584, 658, 728, 743, 761, 764, 765, 800, 810, 
#              812, 814, 818, 824, 829, 830, 870, 871, 877, 881,882, 885, 889, 898, 900, 902, 905, 918, 933, 
#               934,  935,  944,  945,  955,  956,  976,  977,
#         978,  979,  987,  998,  999, 1000, 1001, 1002, 1017, 1020,
#        1041, 1044, 1056, 1059, 1063, 1076, 1077, 1082, 1084, 1085,
#        1086, 1088, 1089, 1104, 1114, 1132, 1161, 1162, 1163, 1164, 1165,
#        1166, 1167, 1172, 1228, 1235, 1237, 1264, 1279, 1541, 1552, 1553,
#        1556, 1557, 1568, 1570, 1571, 1572, 1592, 1594, 1595, 1649, 1696,
#        1745, 1775, 1779, 1786, 1787, 1788, 1789, 1808, 1809, 1816, 1817,
#        1840, 1841, 1846, 1848, 1904, 1912, 1940, 1941, 1948, 1949, 1952,
#        1956, 1960, 1964, 1973, 1981, 1986, 1990, 1994, 1998, 2004, 2012]

msg_unique = [464, 705, 562, 800, 416, 417, 418, 419]
sig_len = np.linspace(1,16, 16, dtype=int)
sig_len = np.append(sig_len, 32)

for mID in msg_unique:
   
    picfolder = "/home/ivory/VersionControl/CodeExample/ExhaustiveSearch/{}".format(mID)
    if not os.path.exists(picfolder):
        try:
            os.mkdir(picfolder)
        except OSError:
            print("[ERROR] Failed to create the data folder {0}.".format(self.datafolder))

    for i in range(0, 64):
        for j in sig_len:
            dfnew= detect_signal(df, messageID = mID, signal_pos = i, signal_len = j)
            if dfnew is not None:
                print(dfnew.shape)
                fig, ax = strymread.create_fig(1)
                ax[0].scatter(x ='Time', y = 'Detected_Values', data = dfnew, s = 1 )
                ax[0].legend( ['Message ID:{}, signal pos = {}, signal length = {}'.format(mID,i, j )], loc='upper left')
                ax[0].set_ylim([np.min(dfnew['Detected_Values'])*0.70-15.0, np.max(dfnew['Detected_Values'])*1.30 + 15.0])
                ax[0].set_xlabel('Time')
                ax[0].set_ylabel('Value')
                ax[0].set_title('Exhaustive Search for Signal Detection, Message ID: {}'.format(mID))
                
                fig.savefig("{}/MessageID_{}-SignalPos_{}-SignalLen_{}".format(picfolder, mID, i, j))
                plt.close()
                # camera.snap()

    # anim = camera.animate(blit=True, interval= 1500)
    # anim.save('/home/ivory/VersionControl/CodeExample/Exhaustive_search_{}.mp4'.format(mID))