#ifndef CONFIG_H
#define CONFIG_H

#define OUT_SHM_NAME "/io_out_shmem"
#define OUT_SHM_PATH "/dev/shm/io_out_shmem"

#define OUT_READ_SLEEP_US 500000 // micro-seconds to sleep for output reading
#define OUT_WRITE_SLEEP_US 500000 // micro-seconds to sleep for output writing

#define IN_SHM_NAME "/io_in_shmem"
#define IN_SHM_PATH "/dev/shm/io_in_shmem"

#define IN_WRITE_SLEEP_US 500000 // micro-seconds to sleep for input writing
#define IN_READ_SLEEP_US 500000 // micro-seconds to sleep for input reading

#endif /* CONFIG_H */