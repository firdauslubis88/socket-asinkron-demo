cmake_minimum_required(VERSION 3.10)
project(SocketAsinkronDemo VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/utils)

# Platform-specific settings
if(WIN32)
    # Windows-specific: Link dengan ws2_32 (Winsock2)
    set(SOCKET_LIBS ws2_32)

    # MinGW-specific settings
    if(MINGW)
        add_compile_options(-Wall -Wextra)
    endif()

    # MSVC-specific settings
    if(MSVC)
        add_compile_options(/W4)
        # Disable specific warnings jika perlu
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    endif()
else()
    # Linux/Unix: Tidak perlu link library tambahan
    set(SOCKET_LIBS "")
    add_compile_options(-Wall -Wextra)
endif()

# ============================================
# TCP Examples
# ============================================
add_executable(tcp_server
    src/tcp/tcp_server.cpp
)
target_link_libraries(tcp_server ${SOCKET_LIBS})

add_executable(tcp_client
    src/tcp/tcp_client.cpp
)
target_link_libraries(tcp_client ${SOCKET_LIBS})

# ============================================
# UDP Examples
# ============================================
add_executable(udp_server
    src/udp/udp_server.cpp
)
target_link_libraries(udp_server ${SOCKET_LIBS})

add_executable(udp_client
    src/udp/udp_client.cpp
)
target_link_libraries(udp_client ${SOCKET_LIBS})

# ============================================
# Async Examples
# ============================================
add_executable(async_server
    src/async/async_tcp_server.cpp
)
target_link_libraries(async_server ${SOCKET_LIBS})

add_executable(async_client
    src/async/async_tcp_client.cpp
)
target_link_libraries(async_client ${SOCKET_LIBS})

# ============================================
# Chat Demo (Advanced Example)
# ============================================
add_executable(chat_server
    src/examples/chat_demo/chat_server.cpp
)
target_link_libraries(chat_server ${SOCKET_LIBS})

add_executable(chat_client
    src/examples/chat_demo/chat_client.cpp
)
target_link_libraries(chat_client ${SOCKET_LIBS})

# ============================================
# Installation (optional)
# ============================================
install(TARGETS
    tcp_server tcp_client
    udp_server udp_client
    async_server async_client
    chat_server chat_client
    RUNTIME DESTINATION bin
)

# ============================================
# Custom targets untuk convenience
# ============================================

# Build all TCP examples
add_custom_target(tcp
    DEPENDS tcp_server tcp_client
)

# Build all UDP examples
add_custom_target(udp
    DEPENDS udp_server udp_client
)

# Build all async examples
add_custom_target(async
    DEPENDS async_server async_client
)

# Build chat demo
add_custom_target(chat
    DEPENDS chat_server chat_client
)

# Build everything
add_custom_target(all_examples
    DEPENDS tcp udp async chat
)

# ============================================
# Print build information
# ============================================
message(STATUS "")
message(STATUS "========================================")
message(STATUS "Socket Asinkron Demo - CMake Configuration")
message(STATUS "========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
if(WIN32)
    message(STATUS "Socket Library: ws2_32 (Winsock2)")
else()
    message(STATUS "Socket Library: POSIX sockets")
endif()
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "")
message(STATUS "Targets to build:")
message(STATUS "  - tcp_server, tcp_client")
message(STATUS "  - udp_server, udp_client")
message(STATUS "  - async_server, async_client")
message(STATUS "  - chat_server, chat_client")
message(STATUS "========================================")
message(STATUS "")
