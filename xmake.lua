add_rules("mode.debug", "mode.release")

add_requires("fmt")

target("xplorer")
    set_kind("binary")
    set_languages("c++17")

    add_includedirs("src")
    add_files("src/*.cpp", "src/**/*.cpp")

    on_config(function (target)
        local res_xml = path.absolute("src/ress/resources.gresource.xml")
        local res_dir = path.absolute("src/ress")
        local gen_c = path.join(target:autogendir(), "gresource.c")
        
        os.mkdir(path.directory(gen_c))
        os.runv("glib-compile-resources", {
            "--target=" .. gen_c, 
            "--sourcedir=" .. res_dir, 
            "--generate-source", 
            res_xml
        })
        target:add("files", gen_c)
    end)

    add_packages("fmt")

    on_load(function (target)
        local function add_pkgconfig(pkg)
            local output = os.iorun("pkg-config --cflags --libs " .. pkg)
            assert(output, pkg .. " not found")

            for flag in output:gmatch("%S+") do
                if flag:sub(1, 2) == "-I" then
                    target:add("includedirs", flag:sub(3))
                elseif flag:sub(1, 2) == "-L" then
                    target:add("linkdirs", flag:sub(3))
                elseif flag:sub(1, 2) == "-l" then
                    target:add("links", flag:sub(3))
                elseif flag:sub(1, 2) == "-D" then
                    target:add("defines", flag:sub(3))
                elseif flag:sub(1, 4) == "-Wl," then
                    target:add("ldflags", flag)
                else
                    target:add("cxflags", flag)
                end
            end
        end

        add_pkgconfig("gtk4")
        add_pkgconfig("webkitgtk-6.0")
        add_pkgconfig("vte-2.91-gtk4")
        add_pkgconfig("gtksourceview-5")
    end)