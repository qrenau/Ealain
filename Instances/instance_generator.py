import numpy as np
import subprocess
import os
from modcma import modularcmaes
import ioh
import argparse
import random
import math

wall_percentage = 25
dimension = wall_percentage*100*2
dimension_cam = 10
seed = 42

def art_gallery(x):
    params = ['%.16g' % a for a in x]
    s = subprocess.check_output(['/home/qrenau/Documents/Code/Camera/ealain/build/./num_cam_napier']+["100"]+["5"]+params+["./gen.csv"])
    return float(s)

def create_walls(x):
    with open("gen.csv", 'w') as fd:
        for i in range(len(x)):
            if (i % 2 == 0) and (i != 0):
                fd.write("\n"+str(math.floor(x[i]))+",")
            else:
                fd.write(str(math.floor(x[i]))+",")


def instance_generator(x):
    create_walls(x)
    np.random.seed(seed)
    random.seed(seed)
    upper_bounds = []
    lower_bounds = []
    for i in range(dimension_cam):
        upper_bounds.append(1)
        lower_bounds.append(0)
    ub = np.array(upper_bounds).reshape(dimension_cam,1)
    lb = np.array(lower_bounds).reshape(dimension_cam,1)
    _, f_mid, _ = modularcmaes.fmin(func=art_gallery, dim=dimension_cam, maxfun=1000,lb = lb, ub = ub, bound_correction ="saturate")
    np.random.seed(seed)
    random.seed(seed)
    _, f_full, _ = modularcmaes.fmin(func=art_gallery, dim=dimension_cam, maxfun=2000,lb = lb, ub = ub, bound_correction ="saturate")
    return 10000 - abs(f_mid-f_full)


upper = []
lower = []
for i in range(dimension):
    upper.append(99)
    lower.append(0)
u = np.array(upper).reshape(dimension,1)
l = np.array(lower).reshape(dimension,1)
_, f, _ = modularcmaes.fmin(func=instance_generator, dim=dimension, maxfun=500,lb = l, ub = u, bound_correction ="saturate",elitist=True)
print(f)
