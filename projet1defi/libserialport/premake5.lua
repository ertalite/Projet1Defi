
baseName = path.getbasename(os.getcwd());

project (baseName)
    kind "StaticLib"
    location "../_build"
    targetdir "../_bin/%{cfg.buildcfg}"
    objdir "../x64/Debug"

    undefines { "DEBUG" }
    filter "configurations:Debug"
        defines { "_DEBUG" }

    filter "action:vs*"
        defines{"LIBSERIALPORT_MSBUILD"}
        links { "setupapi" }

    vpaths 
    {
        ["Header Files/*"] = { "libserialport.h", "libserialport_internal.h"},
        ["Source Files/*"] = { "serialport.c", "timing.c", "windows.c"},
    }
    files {"**.c", "**.cpp", "**.h", "**.hpp"}

    includedirs { "./" }
    includedirs { "./include" }
	