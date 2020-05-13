# Step motor control
* DRV8847(S) is a H-bridge IC to drive step motor.
* AS5047D is a motor encoder to record the step motor's position.
* AD8206 is a differential amplerfier

---

## Collect raw data
```
make clt_raw mk={file_mark} len={length} sz={data_size}
```  
"file_mark" is the marker that data will save to "raw_data_{file_mark}.txt"  
"length" is the length that print size, example: ", 12, 32, \r\n" => len=4  
"data_size" is the data size you want to save.

---

## Collect FIR filter data
Main code is test_id_filter.c  
Collect command example is below:  
```
make clt_raw mk={file_mark} len={length} sz={data_size}
```

---

## Collect motor step response or dynamic data
```
make clt_motor_id mk={file_mark} sz={data_size}
```  
"file_mark" is the marker that data will save to "raw_data_{file_mark}.txt"  
"data_size" is the data size you want to save.  

### Collect step response
1. Setting test_step_res.c, open phase A or B, and then compile and download
2. Go to command line, run `make clt_motor_id mk=step_test sz=1024`
3. After save the data, run `make hdl_step_res mk=step_test`
4. Open data/motor_id_phase_step_step_test.txt, it will identify motor inductance and resistor

### Collect motor full step data
1. Compile and Download test_id_motor_kt.c
2. Go to command line, run `make clt_motor_id mk=motor_test sz=1200`
3. After save the data, run `make hdl_motor_id mk=motor_test`



