Read refcount.[ch] after 15_3_extension-lists.c.

To investigate search trees:
uftrace record -A decref_ctx@arg1 -A decref_ctx@arg2 -A free_node@arg1 -A free_node@arg2 ./112_strees
uftrace replay

To see a sample open assets/run.txt!
