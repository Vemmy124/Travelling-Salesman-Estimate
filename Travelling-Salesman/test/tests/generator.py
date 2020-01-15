#! /usr/bin/python3

import scipy.stats as sps

n = 10000

with open('normal/004', 'w') as inp:
    sample = sps.uniform(30, 5).rvs(size=(n, 2))
    for xx, yy in sample:
        inp.write(f'{xx} {yy}\n')

