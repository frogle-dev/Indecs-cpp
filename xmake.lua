add_rules("mode.debug", "mode.release", "plugin.compile_commands.autoupdate")

set_languages("c++23")
set_warnings("all", "extra", "error", "pedantic")
set_defaultmode("debug")

if is_mode("debug") then
  set_policy("build.sanitizer.address", true)
  set_policy("build.sanitizer.undefined", true)
end

add_requires("catch2")

target("Indecs")
set_kind("headeronly")
add_headerfiles("include/indecs.hpp")
add_includedirs("include", { public = true })

target("Tests")
set_kind("binary")
set_default(false) -- has to be run expicitly with xmake test
add_files("tests/*.cpp")
add_deps("Indecs")
add_packages("catch2")
add_tests("default")
