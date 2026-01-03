cmake_minimum_required(VERSION 3.10)
project(tlist-deprecated C) # Adicionado 'C' para clareza

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# IMPORTANTE: Removidas as linhas de LIBRARY_OUTPUT_PATH para não conflitar com o vcpkg

add_library(Tlist STATIC src/Tlist.c src/Titerator.c)

# Dica: -Werror pode travar o build se houver um aviso bobo no MinGW. 
# Remova o -Werror se o erro persistir.
target_compile_options(Tlist PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(Tlist PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# --- REGRAS DE INSTALAÇÃO (Obrigatório para o vcpkg) ---

install(DIRECTORY include/ DESTINATION include/tlist-deprecated)

# Também ajuste o target se necessário
install(TARGETS Tlist
    EXPORT TlistConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
