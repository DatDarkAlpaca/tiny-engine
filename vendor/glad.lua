project "glad"
    kind "StaticLib"
    language "C"
    warnings "off"
    staticruntime "off"
    systemversion "latest"
    
    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    files {
        "glad/include/glad/glad.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/glad.c"
    }

    includedirs {
        "glad/include"
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter { }
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        inlining "auto"
    filter { }
