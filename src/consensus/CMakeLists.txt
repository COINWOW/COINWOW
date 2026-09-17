add_library(consensus
    merkle.cpp
    tx_check.cpp
    tx_verify.cpp
)

target_include_directories(consensus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

