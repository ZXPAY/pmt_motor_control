#ifndef DRV8847_S_H
#define DRV8847_S_H

/* Define I2C address */
#define DRV8847S_ADDRESS       (0x60)
#define DRV8847S_IC1_CON       (0x01)
#define DRV8847S_IC2_CON       (0x02)
#define DRV8847S_SLR_STATUS    (0x03)
#define DRV8847S_STATUS2       (0x04)

#define IC1_TRQ_POS            7
#define IC1_TRQ_MASK           (1<<IC1_TRQ_POS)
#define IC1_TRQ_FULL           (0<<IC1_TRQ_POS)
#define IC1_TRQ_HALF           (1<<IC1_TRQ_POS)

#define IC1_MODE_POS           0
#define IC1_MODE_MASK          (3<<IC1_MODE_POS)
#define IC1_MODE_4PIN          (0<<IC1_MODE_POS)
#define IC1_MODE_2PIN          (1<<IC1_MODE_POS)
#define IC1_MODE_PARALLEL      (2<<IC1_MODE_POS)
#define IC1_MODE_INDEPENDENT   (3<<IC1_MODE_POS)

#endif /* DRV8847_S_H */
