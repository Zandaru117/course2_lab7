cmake_minimum_required(VERSION 3.10)
# Убирает предупреждение о FindBoost
cmake_policy(SET CMP0167 OLD) 

project(bayan)

set(CMAKE_CXX_STANDARD 17)

# Ищем компоненты Boost
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

# ВАЖНО: убедитесь, что здесь указаны ПРАВИЛЬНЫЕ имена файлов из вашего проекта
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