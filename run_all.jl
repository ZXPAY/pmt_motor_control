run(`echo Start handle image`);

#= Start handle open loop =#
for i = 1:2
    for N_STEP=0:5
        temp = 2^N_STEP
        command = `make hdl mk=ol_n$temp\_$i`
        run(`echo ==========================`)
        run(`echo $command`)
        run(command)
    end
end

#= Start handle close loop, exi_angle =#
for i = 1:2
    for N_STEP=0:5
        temp = 2^N_STEP
        command = `make hdl mk=cl_n$temp\_$i`
        run(`echo ==========================`)
        run(`echo $command`)
        run(command)
    end
end


#= Start handle close loop, exi_angle + current =#
for i = 1:4
    for N_STEP=0:5
        temp = 2^N_STEP
        command = `make hdl mk=cl_in$temp\_$i`
        run(`echo ==========================`)
        run(`echo $command`)
        run(command)
    end
end


#= ================================================== =#

run(`echo ID FIR filter`)
run(`make hdl_filter mk=enc_noinput_10k`)
run(`make hdl_filter mk=enc_noinput_10k_1`)
run(`make hdl_filter mk=enc_noinput_10k_2`)
for i=0:3
    run(`echo ================================`)
    run(`make hdl_filter mk=enc_noinput_40k_$i`)
end

#= ================================================== =#

run(`echo ID motor inductance and resistor`)
for i=0:10
    run(`echo ================================`)
    run(`echo Phase A $i`)
    run(`make hdl_step_res mk=step_a_$i`)
    run(`echo Phase B $i`)
    run(`make hdl_step_res mk=step_b_$i`)
end

