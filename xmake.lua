-- Recursively traverse all folders and their subfolders. If .h or .hpp files are found, add the folder to the project.
function add_includedirs_recursively(dir)
    local has_header_files = false
    for _, file in ipairs(os.files(path.join(dir, "*.h"))) do
        has_header_files = true
        break
    end
    for _, file in ipairs(os.files(path.join(dir, "*.hpp"))) do
        has_header_files = true
        break
    end
    if has_header_files then
        add_includedirs(dir, { public = true })
        add_headerfiles(dir .. "/*.h", { public = true })
    end
    
    for _, subdir in ipairs(os.dirs(path.join(dir, "*"))) do
        add_includedirs_recursively(subdir)
    end
end 


-- Recursively traverse all folders under the folder, find .cpp files and add them to the project.
function add_files_recursively(dir)
    for _, filepath in ipairs(os.files(path.join(dir, "**.cpp"))) do
        add_files(filepath)
    end
    -- For moc, add .h files
    for _, filepath in ipairs(os.files(path.join(dir, "**.h"))) do
        add_files(filepath)
    end
    -- Recursively traverse subfolders
    for _, subdir in ipairs(os.dirs(path.join(dir, "*"))) do
        add_files_recursively(subdir)
    end
end

-- Recursively traverse all folders under the folder, find .qrc files and add them to the project.
function add_qrc_files_recursively(dir)
    for _, filepath in ipairs(os.files(path.join(dir, "**.qrc"))) do
        add_files(filepath, {unique = true})
    end
    -- Recursively traverse subfolders
    for _, subdir in ipairs(os.dirs(path.join(dir, "*"))) do
        add_qrc_files_recursively(subdir)
    end
end

-- Include all deps under the folder
function include_deps(dir)
    for _, file in ipairs(os.files(path.join(dir, "*/xmake.lua"))) do
        local folder_name = path.basename(path.directory(file))
        includes(file)
    end
end


add_rules("mode.debug", "mode.release")

add_requires("gtest", {configs = {shared = true}})

target("GFCEditor")
    add_rules("qt.widgetapp")
    
    -- add src and test files
    add_includedirs_recursively("src")
    add_files_recursively("src")
    add_files_recursively("test")

    -- normal config
    set_rundir(".")
    set_languages("cxx17")

    -- add deps
    add_packages("gtest") 

    includes("vendor/QtCustomTitlebarWindow")
    add_deps("CustomWindow")

    includes("vendor/AdvancedDockingSystem")
    add_deps("ADS")

    -- add resources
    add_qrc_files_recursively("assets")

    if is_plat("windows") then
        set_toolchains("msvc", {vs="2022"})
        add_cxxflags("/utf-8", "/std::c++17")
    end