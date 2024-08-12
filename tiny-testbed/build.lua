create_project("tiny-testbed", "ConsoleApp")
links { "tiny-engine" }

includedirs { 
    "%{wks.location}/tiny-engine/",
    "%{wks.location}/tiny-engine/src", 
}