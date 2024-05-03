add_requires("nlohmann_json")
add_requires("sqlitecpp")
add_requires("gtkmm-3.0", {system = true})
add_requires("gstreamer-1.0", {system = true})
add_requires("sqlite3", {system = true})
add_requires("curlpp", {configs = {shared = false}})
add_requires("libcurl", {system = true})
add_requires("libxapp2", {system = false})

package("libxapp2")
    add_urls("https://github.com/linuxmint/xapp.git")
    add_versions("22", "6b6e78de8d57f86bd81e29be995d677415b23938")

    on_install(function(package)
        -- Read the meson.build file
        local meson_file = io.open("meson.build", "r")
        local meson_content = meson_file:read("*all")
        meson_file:close()

        -- Replace the meson_version line
        meson_content = meson_content:gsub("meson_version : '>=0.56.0'", "meson_version : '>=0.52.0'")

        -- Write the modified meson.build file
        meson_file = io.open("meson.build", "w")
        meson_file:write(meson_content)
        meson_file:close()

        -- Proceed with the installation
        local configs = {}
        table.insert(configs, "-Dapp-lib-only=true")
        table.insert(configs, "-Dstatus-notifier=false")
        table.insert(configs, "-Dpy-overrides-dir=" .. package:buildir())
        import("package.tools.meson").install(package, configs)
    end)
package_end("libxapp2")

add_rules("mode.release")

local objs = {}
for _, file in ipairs(os.files("src/*.cpp")) do
    local name = path.basename(file)
    table.insert(objs, name)
    target(name)
        set_kind("object")
        add_files(file)
        add_packages("gstreamer-1.0")
        add_packages("gtkmm-3.0")
        add_packages("libcurl")
        add_packages("curlpp")
        add_packages("sqlite3")
        add_packages("sqlitecpp")
        add_packages("nlohmann_json")
        add_includedirs("include")
end

for _, file in ipairs(os.files("debug/*.cpp")) do
    local name = path.basename(file)
    target(name)
        set_default(false)
        add_files(file)
        for _, obj in ipairs(objs) do
            add_deps(obj)
        end
        add_includedirs("include")
        add_packages("gstreamer-1.0")
        add_packages("gtkmm-3.0")
        add_packages("libcurl")
        add_packages("curlpp")
        add_packages("sqlite3")
        add_packages("sqlitecpp")
        add_packages("nlohmann_json")
        add_syslinks("pthread")
end

target("radihypn")
    set_kind("binary")
    add_files("src/main.cxx")
    for _, obj in ipairs(objs) do
        add_deps(obj)
    end
    add_packages("gstreamer-1.0")
    add_packages("gtkmm-3.0")
    add_packages("libcurl")
    add_packages("curlpp")
    add_packages("sqlite3")
    add_packages("sqlitecpp")
    add_packages("nlohmann_json")
    add_packages("libxapp2")
    add_includedirs("include")
    add_syslinks("pthread")
