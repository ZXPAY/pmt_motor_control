# Step motor control
* DRV8847(S) is a H-bridge IC to drive step motor.
* AS5047D is a motor encoder to record the step motor's position.
* AD8206 is a differential amplerfier



## Collect data
```makefile
make clt_raw mk={file_mark} len={length} sz={data_size}
```
"file_mark" is the marker that data will save to "raw_data_{file_mark}.txt".

"length" is the length that print size, example: ", 12, 32, \r\n" => len=4.

"data_size" is the data size you want to save.


## Collect full step, step response data
```makefile
make clt_step_res mk={file_mark} sz={data_size}
```
"file_mark" is the marker that data will save to "raw_data_{file_mark}.txt".  s

"data_size" is the data size you want to save.
