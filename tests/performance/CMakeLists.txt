
add_executable( ambient.gemm ambient.gemm.cpp )
add_executable( ambient.lq   ambient.lq.cpp   )
add_executable( ambient.qr   ambient.qr.cpp   )

foreach( performance_test
         ambient.gemm
         ambient.qr
         ambient.lq
       )
    target_link_libraries (${performance_test} ambient)
endforeach(performance_test)
