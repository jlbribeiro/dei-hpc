#!/usr/bin/env python

import pyopencl as cl
import pyopencl.array as cla
import numpy as n

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)

a = n.array([0], dtype=n.int32)

a_array = cla.to_device(queue, a)

prg = cl.Program(ctx, """
	__kernel void test1(__global int* a) {
		a[0] = 1;
	}
	""").build()

event = prg.test1(queue, (1,), None, a_array.data)
event.wait()

a = a_array.get()

print(a)