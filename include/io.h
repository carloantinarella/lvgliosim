#ifndef IO_H
#define IO_H

// TERMINAL part

/*
 * Initialize threads
 */
int terminal_io_init(void);

/*
 * Return digital output value
 */
int get_o1(void);
int get_o2(void);
int get_o3(void);

/*
 * Return analog output value
 */
int get_ana1(void);

/*
 * Set input value
 */
void set_i1(int);
void set_i2(int);
void set_i3(int);

/*
 * Set analog input value
 */
void set_pot1(int);


/*************************************
// MASTER part
**************************************/

/*
 * Initialize shared memory writing for outputs
 */
int master_out_initialization(void);

/*
 * Thread for writing outputs
 */
void* master_out_write(void*);

/*
 * Write output value
 */
void o1(int);
void o2(int);
void o3(int);


#endif /* IO_H */