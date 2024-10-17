add_rules("mode.debug", "mode.release")

target("ADS")
    -- For shared export lib
    add_rules("qt.shared")

    -- For vs sln 
    add_headerfiles("include/ads/*.h")

    -- For qt moc
    add_files(("include/ads/*.h"))

    -- Add src
    add_files("src/*.cpp")
    add_includedirs("include", { public = true })

    -- Add qt frameworks
    add_frameworks("QtGui", "QtWidgets")

    -- Config
    set_toolchains("msvc", {vs="2022"})
    add_cxxflags("/utf-8") 

    set_languages("cxx17")

    if is_plat("windows") then
        add_cxflags("/Zc:__cplusplus")
        add_defines("ADS_EXPORT")
        add_links("Dwmapi","User32", "UxTheme")
    end