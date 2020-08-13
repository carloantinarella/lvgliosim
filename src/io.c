#include "io.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "config.h"
#include "shdata.h"

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


static int terminal_out_shm_fd;         // shared memory file descriptor for outputs (terminal)
static out_t* output_data;              // pointer to shared memory for output
static out_t terminal_output_data_copy; // struct containing the read output data

static int terminal_in_shm_fd;          // shared memory file descriptor for inputs (terminal)
static in_t* input_data;                // pointer to shared memory for input
static in_t terminal_input_data_copy;   // struct containing the written input data

static int master_out_shm_fd;           // shared memory file descriptor for outputs (master)
static out_t* master_output_data;       // pointer to shared memory
static out_t master_output_data_copy;   // struct containing the data to be written to memory

/*
 * Initialize shared memory reading for outputs
 */
int terminal_out_initialization(void);

/*
 * Thread for reading outputs
 */
void* terminal_out_read(void*);

/*
 * Initialize shared memory writing for inputs
 */
int terminal_in_initialization(void);

/*
 * Thread for writing inputs
 */
void* terminal_in_write(void*);

int terminal_io_init(void)
{
    if (terminal_out_initialization() == -1)
        return -1;
    printf("Starting OUT reading thread...\n");
    pthread_t out_read_th;
    pthread_attr_t out_read_th_att;
    pthread_attr_init(&out_read_th_att); // default thread attributes 
    pthread_create(&out_read_th, &out_read_th_att, terminal_out_read, NULL);

    if (terminal_in_initialization() == -1)
        return -1;
    printf("Starting IN reading thread...\n");
    pthread_t in_read_th;
    pthread_attr_t in_read_th_att;
    pthread_attr_init(&in_read_th_att); // default thread attributes
    pthread_create(&in_read_th, &in_read_th_att, terminal_in_write, NULL);

    return 0;
}

int terminal_out_initialization(void)
{
    /* Unlink previously created shared memory, if any */
    shm_unlink(OUT_SHM_NAME);

    /* Initialize shared memory for outputs */
    terminal_out_shm_fd = shm_open(OUT_SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (terminal_out_shm_fd == -1) {
        printf("Error while initializing shared memory for outputs. Exiting...\n");
        return -1;
    }
    if (ftruncate(terminal_out_shm_fd, sizeof(out_t) != 0))
        return -1;

    /* Map shared memory object (read-only) */
    output_data = (out_t *)mmap(NULL, sizeof(out_t), PROT_READ, MAP_SHARED, terminal_out_shm_fd, 0);

    close(terminal_out_shm_fd);

    printf("Output shared memory initialization successful\n");
    return 0;
}

void* terminal_out_read(void* arg)
{
    /* This must be started as thread */
    while(1) {
        memcpy(&terminal_output_data_copy, output_data, sizeof(out_t));
        usleep(OUT_READ_SLEEP_US);
    }
}

int terminal_in_initialization(void)
{
    /* Unlink previously shared memory for outputs */
    shm_unlink(IN_SHM_NAME);

    /* Initialize shared memory for inputs */
    terminal_in_shm_fd = shm_open(IN_SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (terminal_in_shm_fd == -1) {
        printf("Error while initializing shared memory for inputs. Exiting...\n");
        return -1;
    }
    if (ftruncate(terminal_in_shm_fd, sizeof(in_t)) != 0)
        return -1;

    /* Map shared memory object (write-only) */
    input_data = (in_t *)mmap(NULL, sizeof(in_t), PROT_WRITE, MAP_SHARED, terminal_in_shm_fd, 0);
    close(terminal_in_shm_fd);
    printf("Input shared memory initialization successfull\n");
    return 0;
}

void* terminal_in_write(void* arg)
{
    /* This must be started as thread */
    while(1) {
        memcpy(input_data, &terminal_input_data_copy, sizeof(in_t));
        usleep(IN_READ_SLEEP_US);
    }
}


int get_o1(void)
{
    return (terminal_output_data_copy.dig_out & 0x01);
}

int get_o2(void)
{
    return (terminal_output_data_copy.dig_out & 0x02) >> 1;
}

int get_o3(void)
{
    return (terminal_output_data_copy.dig_out & 0x04) >> 2;
}

int get_o4(void)
{
    return (terminal_output_data_copy.dig_out & 0x08) >> 3;
}

int get_ana1(void)
{
    return terminal_output_data_copy.ana_out[0];
}

void set_i1(int i)
{
    printf("I1 = %d\n", i);
    if (i >= 0) 
        terminal_input_data_copy.dig_in |= 0x01;
    else
        terminal_input_data_copy.dig_in &= ~(0x01);
}

void set_i2(int i)
{
    printf("I2 = %d\n", i);
    if (i >= 0)
        terminal_input_data_copy.dig_in |= 0x02;
    else
        terminal_input_data_copy.dig_in &= ~(0x02);
}

void set_i3(int i)
{
    printf("I3 = %d\n", i);
    if (i >= 0)
        terminal_input_data_copy.dig_in |= 0x04;
    else
        terminal_input_data_copy.dig_in &= ~(0x04);
}

void set_i4(int i)
{
    printf("I4 = %d\n", i);
    if (i >= 0)
        terminal_input_data_copy.dig_in |= 0x08;
    else
        terminal_input_data_copy.dig_in &= ~(0x08);
}

void set_pot1(int v)
{
    printf("ANA1 = %d\n", v);
    terminal_input_data_copy.ana_in[0] = v;
}








/*
 ****************
 * MASTER
 ****************
 */

int master_out_initialization(void)
{
    /* Attach to previously existing shared memory for writing */
    master_out_shm_fd = open(OUT_SHM_PATH, O_RDWR);
    if (master_out_shm_fd == -1) {
        printf("Error while trying to access shared memory\n");
        return -1;
    }
    printf("master_out_shm_fd: %d\n", master_out_shm_fd);
    if (ftruncate(master_out_shm_fd, sizeof(out_t)) != 0)
        return -1;
    master_output_data = (out_t *)mmap(NULL, sizeof(out_t), PROT_READ | PROT_WRITE, MAP_SHARED, master_out_shm_fd, 0);
    if (master_output_data == MAP_FAILED) {
        handle_error("mmap");
        printf("Unable to attach to shared memory\n");
        return -1;
    }
    close(master_out_shm_fd);
    printf("Attached to shared memory\n");
    /* Initialize the data copy */
    memset(&master_output_data_copy, 0, sizeof(out_t));
    return 0;
}

void* master_out_write(void *arg)
{
    /* This must be started as thread */
    while(1) {
        memcpy(master_output_data, &master_output_data_copy, sizeof(out_t));
        usleep(OUT_WRITE_SLEEP_US);
    }
}

void o1(int o)
{
    if (o >= 1)
        master_output_data_copy.dig_out |= 0x01;
    else
        master_output_data_copy.dig_out &= ~(0x01);
}

void o2(int o)
{
    if (o >= 1)
        master_output_data_copy.dig_out |= 0x02;
    else
        master_output_data_copy.dig_out &= ~(0x02);
}

void o3(int o)
{
    if (o >= 1)
        master_output_data_copy.dig_out |= 0x04;
    else
        master_output_data_copy.dig_out &= ~(0x04);
}