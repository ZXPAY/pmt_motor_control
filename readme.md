# Step motor control

## Collect data
```
clt_raw mk={file_mark} len={length} sz={data_size}
```
"file_mark" is the marker that data will save to "raw_data_{file_mark}.txt".
"length" is the length that print size, example: ", 12, 32, \r\n" => len=4.
"data_size" is the data size you want to save.  

## Collect full step, step response data
```
make clt_step_res
```

