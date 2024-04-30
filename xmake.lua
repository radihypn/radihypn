add_requires("nlohmann_json")
add_requires("sqlitecpp")
add_requires("utest.h")
add_requires("gtkmm-3.0", {system = true})
add_requires("gstreamer-1.0", {system = true})
add_requires("sqlite3", {system = true})
add_requires("curlpp")
add_requires("libcurl", {system = true})


add_rules("mode.release")

target("radihypn")
    set_kind("binary")
    add_files("main.cpp")
    add_ldflags("-lpthread")
    add_packages("gstreamer-1.0")
    add_packages("gtkmm-3.0")
    add_packages("libcurl")
    add_packages("curlpp")
    add_packages("sqlite3")
    add_packages("sqlitecpp")
    add_packages("nlohmann_json")
    add_packages("utest.h")
    add_installfiles("misc/*.desktop", {prefixdir = "share/applications"})
    add_installfiles("misc/*.png", {prefixdir = "share/icons"})


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
        add_packages("curlpp")
        add_packages("sqlite3")
        add_packages("sqlitecpp")
        add_packages("nlohmann_json")
        add_packages("utest.h")
        add_ldflags("-lpthread") -- todo: test if we can replace it with add_syslinks("pthread")
end


