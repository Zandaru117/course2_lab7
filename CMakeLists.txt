cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0167 OLD) 

project(bayan)

set(CMAKE_CXX_STANDARD 17)


find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})


add_executable(bayan 
    main.cpp 
    scanner.cpp 
    controller.cpp 
    mask.cpp 
    hash.cpp
)

target_link_libraries(bayan 
    ${Boost_LIBRARIES} 
    OpenSSL::SSL 
    OpenSSL::Crypto
)