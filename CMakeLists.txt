cmake_minimum_required(VERSION 3.16)

project(BACMSystem VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(bacm_system
    "BACM System.cpp"
    Account.cpp
    AccountManager.cpp
    Address.cpp
    CompanyBST.cpp
    Contact.cpp
    CsvUtils.cpp
    DateTimeUtils.cpp
    InputUtils.cpp
    StateValidator.cpp
)
