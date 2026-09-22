cmake_minimum_required(VERSION 3.10)
project(CalculatorProject CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Створення статичної бібліотеки з файлу calculator.cpp та calculator.h
add_library(calculator STATIC calculator.cpp calculator.h)

#Створення виконуваного файлу завдяки main.cpp
add_executable(app main.cpp)

#Зв'язування виконуваного файлу зі створеною бібліотекою
target_link_libraries(app PRIVATE calculator)
