add_requires("nlohmann_json")
add_requires("sqlitecpp")
add_requires("utest.h")
add_requires("gtkmm-3.0", {system = true})
add_requires("gstreamer-1.0", {system = true})
add_requires("sqlite3", {system = true})
add_requires("curlpp-static")
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

package("curlpp-static")
    set_homepage("http://www.curlpp.org")
    set_description("C++ wrapper around libcURL")
    set_license("MIT")

    add_urls("https://github.com/jpbarrette/curlpp.git")
    add_versions("2023.07.27", "1d8c7876cc81d7d125b663066282b207d9cbfe9a")

    add_deps("libcurl")

    on_install("windows", "linux", "macosx", "mingw", "cross", function (package)
        io.writefile("xmake.lua", ([[
            set_languages("c++11")
            add_rules("mode.debug", "mode.release")
            add_requires("libcurl")
            target("curlpp")
                set_kind("static")
                add_files("src/**.cpp")
                add_includedirs("include/")
                add_headerfiles("include/(**.hpp)", "include/(**.inl)")
                add_packages("libcurl")
                if is_plat("windows") and is_kind("shared") then
                    add_rules("utils.symbols.export_all", {export_classes = true})
                end
        ]]))
        import("package.tools.xmake").install(package)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            int main() {
                curlpp::Cleanup cleaner;
                curlpp::Easy request;
                request.setOpt<curlpp::options::Url>("https://example.com");
            }
        ]]}, { configs = {languages = "c++11"}, includes = {"curlpp/cURLpp.hpp", "curlpp/Easy.hpp", "curlpp/Options.hpp"}
        }))
    end)
package_end("curlpp-static")

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
        add_packages("curlpp-static")
        add_packages("sqlite3")
        add_packages("sqlitecpp")
        add_packages("nlohmann_json")
        add_packages("utest.h")
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
        add_packages("curlpp-static")
        add_packages("sqlite3")
        add_packages("sqlitecpp")
        add_packages("nlohmann_json")
        add_packages("utest.h")
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
    add_packages("curlpp-static")
    add_packages("sqlite3")
    add_packages("sqlitecpp")
    add_packages("nlohmann_json")
    add_packages("utest.h")
    add_packages("libxapp2")
    add_includedirs("include")
    add_syslinks("pthread")
