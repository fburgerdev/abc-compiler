-- premake5.lua
ROOT = ".."

-- workspace
workspace "abc"
    -- startproject
    startproject "abc"
    -- cpp
    language "C++"
    cppdialect "C++20"

    -- debug
    debugger "GDB"

    -- defines
    defines {  }

    -- dependancies
    -- :: directories
    libdirs {
        --   ROOT .. "/lib/%{cfg.buildcfg}",
        --   ROOT .. "/modules/*/lib/%{cfg.buildcfg}",
        --   ROOT .. "/vendor/*/lib/%{cfg.buildcfg}"
    }
    -- :: libraries
    links {  --[[ INSERT ADDITIONAL LINKS HERE ]] }

    -- config
    configurations { "debug", "release", "dist" }
    -- :: debug
    filter "configurations:debug"
        -- precompiled headers
        pchheader "common.hpp"
        pchsource "common.cpp"
        -- symbols / defines
        symbols "On"
        defines { "CONFIG_DEBUG" }
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
    
    -- toolset
    -- :: gcc
    filter "toolset:gcc"
        buildoptions { "-Wall", "-Wextra", "-Wpedantic" }

-- project lib
project "abc"
    -- console
    kind "ConsoleApp"

    -- include
    includedirs {
        ROOT,
        ROOT .. "/src",
        --   ROOT .. "/modules/*/include",
        --   ROOT .. "/vendor/*/include"
    }
    -- files
    files {
        ROOT .. "/src/**",
        --   ROOT .. "/vendor/*/src/**",
    }
   
    -- binaries
    targetdir(ROOT .. "/bin/%{cfg.system}_%{cfg.buildcfg}")
    objdir(ROOT .. "/bin/%{cfg.system}_%{cfg.buildcfg}/obj")