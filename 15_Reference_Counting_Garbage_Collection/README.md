- Read `refcount.[ch]` after `15_3_extension-lists.c`.

- To investigate search trees:
```shell
uftrace record -A decref_ctx@arg1 -A decref_ctx@arg2 -A free_node@arg1 -A free_node@arg2 ./15_4_strees
uftrace replay
```

- To see a sample open `assets/run.txt`.

- Links
  - [Reference counting lists in C](https://mailund.dk/posts/c-refcount-list/) by the author.
  - [Generic C Reference Counting](https://nullprogram.com/blog/2015/02/17/)
