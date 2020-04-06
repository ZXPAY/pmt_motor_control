#ifndef CONTROL_H
#define CONTROL_H


/* All control object initialize */
void control_init(void);

/* Print all parameters via RS485 */
void control_print(void);

/* Calculate control duty */
void control_handle(void);



#endif /* CONTROL_H */
