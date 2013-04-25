#!/usr/bin/env python

import pyopencl as cl
import numpy as n
import struct as s

ctx = cl.create_some_context()
queue = cl.CommandQueue(ctx)

for use_struct in (True, False):

	if use_struct:
		a = s.pack('=bi',1,2)
		print(a, len(a))
		a_dev = cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, len(a))
	else:
		a = n.array([(1,2)], dtype=n.dtype('i1i4', align=True))
		# this no longer works - without align=True we get the wrong value
		# a = n.array([(1,2)], dtype=n.dtype('i1i4'))
		print(a, a.itemsize, a.nbytes)
		a_dev = cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, a.nbytes)

	b = n.array([0], dtype='i4')
	print(b, b.itemsize, b.nbytes)
	b_dev = cl.Buffer(ctx, cl.mem_flags.READ_ONLY, b.nbytes)

	c = n.array([0], dtype='i4')
	print(c, c.itemsize, c.nbytes)
	c_dev = cl.Buffer(ctx, cl.mem_flags.READ_ONLY, c.nbytes)

	prg = cl.Program(ctx, """
		typedef struct s {
			char f0;
			int f1 __attribute__ ((packed));
		} s;
		__kernel void test(__global const s *a, __global int *b, __global
		int *c) {
			*b = a->f0;
			*c = a->f1;
		}
		""").build()

	cl.enqueue_copy(queue, a_dev, a)
	event = prg.test(queue, (1,), None, a_dev, b_dev, c_dev)
	event.wait()
	cl.enqueue_copy(queue, b, b_dev)
	print(b)
	cl.enqueue_copy(queue, c, c_dev)
	print(c)