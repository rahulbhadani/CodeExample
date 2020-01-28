# Author: Rahul Bhadani
import numpy
import os
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import make_classification

rfCf = RandomForestClassifier()
print(rfCf)


print('Number of Cores = ', os.cpu_count())