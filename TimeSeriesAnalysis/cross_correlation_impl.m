%(C) Rahul Bhadani
data_src_1  = '/home/ivory/VersionControl/catvehicle_ws/src/sparkle/src/Circle_Test_n_10_updateRate_1_max_update_rate_25_time_step_0.01_logtime_300_laser_true_2019-12-19-11-28-56/magna-setvel.csv';
data_src_2 = '/home/ivory/VersionControl/catvehicle_ws/src/sparkle/src/Circle_Test_n_2_updateRate_1_max_update_rate_25_time_step_0.01_logtime_300_laser_true_2019-12-19-01-36-42/magna-setvel.csv';

data_src_3 = '/home/ivory/VersionControl/catvehicle_ws/src/sparkle/src/Circle_Test_n_25_updateRate_25_max_update_rate_25_time_step_0.01_logtime_300_laser_false_2019-12-18-23-03-12/magna-setvel.csv';

metric = cross_correlation(data_src_1, data_src_2);