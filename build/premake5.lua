-- premake5.lua
ROOT = ".."
WORKSPACE = "abc"

-- workspace
workspace(WORKSPACE)
    -- startproject
    startproject(WORKSPACE)
    -- cpp
    language "C++"
    cppdialect "C++20"

    -- debug
    debugger "GDB"

    -- defines
    defines { }

    -- dependancies
    -- :: directories
    libdirs {
        --[[ INSERT ADDITIONAL LINKS HERE ]]
    }
    -- :: libraries
    links { --[[ INSERT ADDITIONAL LINKS HERE ]] }

    -- config
    configurations { "debug", "release", "dist" }
    -- :: debug
    filter "configurations:debug"
        -- symbols / defines
        symbols "On"
        defines { "CONFIG_DEBUG" }
        -- options
        linkoptions{ }
    -- :: release
    filter "configurations:release"
        -- optimize / defines
        optimize "On"
        defines { "CONFIG_RELEASE" }
        -- options
        linkoptions{ "-Ofast" }
    -- :: dist
    filter "configurations:dist"
        -- optimize / defines
        optimize "On"
        defines { "CONFIG_DIST" }
        -- options
        linkoptions { "-Ofast" }
    
    -- system
    -- :: windows
    filter "system:windows"
        defines { "SYSTEM_WINDOWS" }
    -- :: linux
    filter "system:linux"
        defines { "SYSTEM_LINUX" }

    -- binaries
    targetdir(ROOT .. "/bin/%{cfg.buildcfg}")
    objdir(ROOT .. "/bin/obj")

    -- toolset
    -- :: gcc
    filter "toolset:gcc"
        buildoptions { "-Wall", "-Wextra", "-Wpedantic" }

-- project app
project(WORKSPACE)
    -- console
    kind "ConsoleApp"

    -- precompiled headers
    pchheader "common.hpp"
    pchsource "common.cpp"
    
    -- include
    includedirs {
        ROOT .. "/",
        ROOT .. "/src",
    }
    -- files
    files {
        ROOT .. "/src/**",
    }