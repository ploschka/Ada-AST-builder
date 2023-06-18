cmake_minimum_required(VERSION 3.8)
project(Ada_AST_builder
    VERSION 0.1.0
    DESCRIPTION "Abstract syntax tree builder for Ada language"
    LANGUAGES CXX)

include_directories(include)

set(exename adast)

set(tokenlib src/adast/Token.cpp)
set(astlib src/adast/ASTNode.cpp src/adast/AST.cpp)
set(lexlib src/adast/Lexer.cpp src/adast/LexerFactory.cpp src/adast/LexerStates.cpp src/adast/FileData.cpp)
set(parslib src/adast/Parser.cpp src/adast/ParserFactory.cpp)

add_library(token STATIC ${tokenlib})
add_library(ast STATIC ${astlib})
add_library(lexer STATIC ${lexlib})
add_library(parser STATIC ${parslib})

target_link_libraries(lexer token)
target_link_libraries(parser lexer ast token)

set(libs lexer parser)

add_executable(${exename} main.cpp)
set_property(TARGET ${exename} PROPERTY CXX_STANDARD 17)

if(MSVC)
  target_compile_options(${exename} PRIVATE /W4 /WX)
else()
  target_compile_options(${exename} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_link_libraries(${exename} ${libs})
