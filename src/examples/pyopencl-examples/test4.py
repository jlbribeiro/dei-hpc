#!/usr/bin/env python

import pyopencl as cl
import numpy as n

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)

a = n.array([0], dtype=n.int32)
b = n.uint8(129)

a_dev = cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, a.nbytes)

prg = cl.Program(ctx, """
	__kernel void test1(__global int* a, const uchar b) {
		a[0] = b;
	}
	""").build()

event = prg.test1(queue, (1,), None, a_dev, b)
event.wait()

cl.enqueue_copy(queue, a, a_dev)

print(a)
