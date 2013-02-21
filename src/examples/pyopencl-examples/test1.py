#!/usr/bin/env python

import pyopencl as cl
import numpy as n

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)

a = n.array([0], dtype=n.int32)

# this works
a_dev = cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, a.nbytes)
# this alternative (plus below) fails
#a_dev = cla.to_device(queue, a)

prg = cl.Program(ctx, """
	__kernel void test1(__global int* a) {
		a[0] = 1;
	}
	""").build()

event = prg.test1(queue, (1,), None, a_dev)
event.wait()

# this works
cl.enqueue_copy(queue, a, a_dev)
# this alternative(plus above) fails
#a = a_dev.get()

print(a)