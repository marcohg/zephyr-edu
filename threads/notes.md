# [Basic thread manipulation]
See also Kernel/Kernel Services/[Threads] documentation

Notes:
- 3 threads spawned at compile using `K_THREAD_DEFINE`
- Threads communicate through [FIFOs] using `K_FIFO_DEFINE`
- #define `PRIORITY 7`: lower numerical values indicates higher priority.
  - cooperative: negative values
  - preemptive: positive 0,1,2,... set `CONFIG_NUM_PREEMPT_PRIORITIES`
- Examples uses `k_malloc`, the `k_free` counter part is on the uart thread.



`K_FIFO_DEFINE(printk_fifo);`  
Statically define and initialize a FIFO queue.  
The FIFO queue can be accessed outside the module where it is defined using: `extern struct k_fifo <name>;` above macro is equivalente to:  
```
struct k_fifo printk_fifo;
k_fifo_init(&printk_fifo);
```

--- 
[Basic thread manipulation]:https://docs.zephyrproject.org/latest/samples/basic/threads/README.html#multi-thread-blinky
[Threads]:https://docs.zephyrproject.org/latest/kernel/services/threads/index.html
[FIFOs]:https://docs.zephyrproject.org/latest/kernel/services/data_passing/fifos.html