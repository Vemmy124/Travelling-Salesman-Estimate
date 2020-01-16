#! /usr/bin/python3

import scipy.stats as sps

n = 10

with open('normal/005', 'w') as inp:
    sample = sps.norm(30, 5).rvs(size=(n, 2))
    for xx, yy in sample:
        inp.write(f'{xx} {yy}\n')

