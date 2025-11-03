#!/usr/bin/env python3


MASK = (1 << 32) - 1
INV33 = pow(33, -1, 1 << 32)  # 0x3E0F83E1


def invert_step(u_next, ch):
    t = (u_next ^ ord(ch)) & MASK
    return (t * INV33) & MASK


GOAL = 0x4415B419
print(invert_step(GOAL, "a"))  # print :
