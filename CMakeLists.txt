cmake_minimum_required(VERSION 3.10)
project(tlist-deprecated C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Criar o target
add_library(Tlist STATIC src/Tlist.c src/Titerator.c)

# Configurar diretórios de include para quem for usar a lib
target_include_directories(Tlist PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/tlist-deprecated> 
)

# Flags de compilação
if(MSVC)
    target_compile_options(Tlist PRIVATE /W4 /WX)
else()
    target_compile_options(Tlist PRIVATE -Wall -Wextra -Wpedantic)
endif()

# --- REGRAS DE INSTALAÇÃO ---

# 1. Instalar os Headers na subpasta tlist-deprecated
install(DIRECTORY include/ 
    DESTINATION include/tlist-deprecated
    FILES_MATCHING PATTERN "*.h"
)

# 2. Instalar o arquivo binário (.lib ou .a)
install(TARGETS Tlist
    EXPORT TlistConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# 3. Gerar arquivos de configuração do CMake (essencial para find_package)
install(EXPORT TlistConfig
    NAMESPACE Tlist::
    DESTINATION share/tlist-deprecated
)
