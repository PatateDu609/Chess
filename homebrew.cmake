set(HOMEBREW_PREFIX "/opt/homebrew"
        CACHE PATH "Path to Homebrew installation")

set(CMAKE_C_COMPILER "${HOMEBREW_PREFIX}/opt/llvm/bin/clang-17")
set(CMAKE_CXX_COMPILER "${HOMEBREW_PREFIX}/opt/llvm/bin/clang-17")

set(CMAKE_PREFIX_PATH
        "${HOMEBREW_PREFIX}"
        "/Library/Frameworks/SDL2.framework"
        "/Library/Frameworks/SDL2.framework/Resources/CMake"
        "/Library/Frameworks/SDL2_ttf.framework"
        "/Library/Frameworks/SDL2_ttf.framework/Resources/CMake"
        # These libraries are keg-only and not loaded into
        # the root prefix by default (to avoid clashes).
        "${HOMEBREW_PREFIX}/opt/llvm/lib/c++"
)

list(TRANSFORM CMAKE_PREFIX_PATH APPEND "/include"
        OUTPUT_VARIABLE CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES "${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES}")

set(CMAKE_FIND_FRAMEWORK NEVER)
set(CMAKE_FIND_APPBUNDLE NEVER)

set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH FALSE)
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)

set(CMAKE_MODULE_PATH /Library/Frameworks/SDL2.framework/Resources/CMake)
#set(CMAKE_LIBRARY_PATH /Library/Frameworks/SDL2.framework)

