import matplotlib.pyplot as plt
import numpy as np
from tool.mypickle import *

DATA_DIR = "data"
pickle_last_nm = "_2"

i1 = load_pickle(DATA_DIR+'/i1'+pickle_last_nm+'.pickle')
i2 = load_pickle(DATA_DIR+'/i2'+pickle_last_nm+'.pickle')
angle = load_pickle(DATA_DIR+'/angle'+pickle_last_nm+'.pickle')
sele_dangle = load_pickle(DATA_DIR+'/sele_dangle'+pickle_last_nm+'.pickle')
cele_dangle = load_pickle(DATA_DIR+'/cele_dangle'+pickle_last_nm+'.pickle')
th_svpwm = load_pickle(DATA_DIR+'/th_svpwm'+pickle_last_nm+'.pickle')
i_svpwm = load_pickle(DATA_DIR+'/i_svpwm'+pickle_last_nm+'.pickle')
th_er = load_pickle(DATA_DIR+'/th_er'+pickle_last_nm+'.pickle')
th_cum = load_pickle(DATA_DIR+'/th_cum'+pickle_last_nm+'.pickle')
pwm1 = load_pickle(DATA_DIR+'/pwm1'+pickle_last_nm+'.pickle')
pwm2 = load_pickle(DATA_DIR+'/pwm2'+pickle_last_nm+'.pickle')







