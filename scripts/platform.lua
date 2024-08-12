function detect_platform()
    filter { "system:Windows" }
        defines {
            "TINY_PLATFORM_WIN32",
            "TINY_WINDOWING_GLFW",
            "GLFW_STATIC"
        }
        links {
            "shcore.lib",
            "dxva2.lib"
        }
    filter { }

    filter { "system:Unix or linux or macosx" }
        defines {
            "TINY_WINDOWING_GLFW",
            "TINY_PLATFORM_OTHER",
            "GLFW_STATIC"
        }
    filter { }
end
