#ifndef FPU_H
#define FPU_H

extern void enable_fpu(void);
extern float f32_add(float f1, float f2);  //  return f1+f2
extern float f32_sub(float f1, float f2);  //  return f1-f2
extern float f32_div(float f1, float f2);  //  return f1/f2
extern float f32_mul(float f1, float f2);  //  return f1*f2
extern float f32_abs(float f1);            //  return positive f1


#endif /* FPU_H */
