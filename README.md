# Skiplist

This is a implementation of skiplist used mostly in index construction, but the implementation is just a data structure that can be used in many scenarios.

The basic idea of skiplist is introducing high-level connections between nodes to speed up the finding or inserting one node. There are several strategies to create high-level connection including randomly assignment or uniformly assignment.

make_zipf.cpp is one dynamic programming algorithm for creating a high-level connection to minimize the whole searching time. The frequencies are given forehead.
