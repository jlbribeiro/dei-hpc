#!/usr/bin/env python

import pyopencl as cl
import numpy as n

'''constants (like b) don't need buffering.'''

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)

a = n.array([0], dtype=n.int32)
b = n.int32(4)

a_dev = cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, a.nbytes)

prg = cl.Program(ctx, """
	__kernel void test1(__global int* a, const int b) {
		a[0] = b;
	}
	""").build()

event = prg.test1(queue, (1,), None, a_dev, b)
event.wait()

cl.enqueue_copy(queue, a, a_dev)

print(a)