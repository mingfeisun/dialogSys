cmake_minimum_required(VERSION 2.8)
project(test_dialog)

find_package(qibuild)

# Create a executable named test_dialog
# with the source file: main.cpp
qi_create_bin(test_dialog "main.cpp")

# Add a simple test:
qi_create_test(test_test_dialog "test.cpp")
