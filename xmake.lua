add_rules("mode.debug", "mode.release")

target("GFCEditor")
    add_rules("qt.widgetapp")
    
    add_headerfiles(
        "src/GLDAttributeArea/*.h",
        "src/GLDAuxiliaryArea/*.h",
        "src/GLDEditor/*.h",
        "src/GLDSchema/*.h",
        "src/GLDToolbar/*.h",
        "src/GLDMainWindow/*.h",
        "src/GLDExpressParser/*.h",
        "src/GLDGfcReader/*.h"
        )

    add_includedirs(
        "src/GLDAttributeArea",
        "src/GLDAuxiliaryArea",
        "src/GLDEditor",
        "src/GLDSchema",
        "src/GLDToolbar",
        "src/GLDMainWindow",
        "src/GLDExpressParser",
        "src/GLDGfcReader"
    )

    add_files(
        "src/main.cpp",
        "src/GLDAttributeArea/*.cpp",
        "src/GLDAuxiliaryArea/*.cpp",
        "src/GLDEditor/*.cpp",
        "src/GLDSchema/*.cpp",
        "src/GLDToolbar/*.cpp",
        "src/GLDMainWindow/*.cpp",
        "src/GLDExpressParser/*.cpp",
        "src/GLDGfcReader/*.cpp"
    )

    add_files("src/GLDMainWindow/mainwindow.ui")

    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files(
        "src/GLDAttributeArea/*.h",
        "src/GLDAuxiliaryArea/*.h",
        "src/GLDEditor/*.h",
        "src/GLDSchema/*.h",
        "src/GLDToolbar/*.h",
        "src/GLDMainWindow/*.h",
        "src/GLDExpressParser/*.h",
        "src/GLDGfcReader/*.h"
    )

    set_rundir(".")
    set_toolchains("msvc", {vs="2022"})
    add_cxxflags("/utf-8", "/std::c++17")
    set_languages("cxx17")

    -- Add Deps
    includes("vendor/QtCustomTitlebarWindow")
    add_deps("CustomWindow")
    
    includes("vendor/AdvancedDockingSystem")
    add_deps("ADS")

    -- add resources
    add_files("vendor/QtCustomTitlebarWindow/assets/images/image.qrc")
    add_files("vendor/QtCustomTitlebarWindow/assets/qss/qss.qrc")

    add_files("vendor/AdvancedDockingSystem/res/ads.qrc")