add_rules("mode.debug", "mode.release")

target("GFCEditor")
    add_rules("qt.widgetapp")
    
    add_headerfiles(
        "src/GLDAttributeArea/*.h",
        "src/GLDAuxiliaryArea/*.h",
        "src/GLDEditor/*.h",
        "src/GLDSchema/*.h",
        "src/GLDToolBar/*.h",
        "src/GLDMainWindow/*.h",
        "src/GLDExpressParser/*.h",
        "src/GLDGfcReader/*.h",
        "src/GLDMenuBar/*.h"
        )

    add_includedirs(
        "src/GLDAttributeArea",
        "src/GLDAuxiliaryArea",
        "src/GLDEditor",
        "src/GLDSchema",
        "src/GLDToolBar",
        "src/GLDMainWindow",
        "src/GLDExpressParser",
        "src/GLDGfcReader",
        "src/GLDMenuBar"
    )

    add_files(
        "src/main.cpp",
        "src/GLDAttributeArea/*.cpp",
        "src/GLDAuxiliaryArea/*.cpp",
        "src/GLDEditor/*.cpp",
        "src/GLDSchema/*.cpp",
        "src/GLDToolBar/*.cpp",
        "src/GLDMainWindow/*.cpp",
        "src/GLDExpressParser/*.cpp",
        "src/GLDGfcReader/*.cpp",
        "src/GLDMenuBar/*.cpp"
    )

    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files(
        "src/GLDAttributeArea/*.h",
        "src/GLDAuxiliaryArea/*.h",
        "src/GLDEditor/*.h",
        "src/GLDSchema/*.h",
        "src/GLDToolBar/*.h",
        "src/GLDMainWindow/*.h",
        "src/GLDExpressParser/*.h",
        "src/GLDGfcReader/*.h",
        "src/GLDMenuBar/*.h"
    )

    set_rundir(".")
    set_toolchains("msvc", {vs="2022"})
    add_cxxflags("/utf-8", "/std::c++17")
    set_languages("cxx17")

    -- Add Deps
    includes("vendor/QtCustomTitlebarWindow")
    add_deps("CustomWindow")
    
    -- includes("vendor/AdvancedDockingSystem")
    -- add_deps("ADS")
    -- 暂不使用ADS库

    -- add resources
    add_files("vendor/QtCustomTitlebarWindow/assets/images/image.qrc")
    add_files("vendor/QtCustomTitlebarWindow/assets/qss/qss.qrc")

    -- add_files("vendor/AdvancedDockingSystem/res/ads.qrc")

    add_files("assets/images/icon.qrc")
    add_files("assets/fonts/font.qrc")
