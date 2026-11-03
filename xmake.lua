add_rules("mode.debug", "mode.release", "plugin.compile_commands.autoupdate")

set_languages("c++23")

set_warnings("all", "extra", "error", "pedantic", "conversion", "shadow")

set_defaultmode("debug")

if is_mode("debug") then
  set_policy("build.sanitizer.address", true)
  set_policy("build.sanitizer.undefined", true)
end

target("Indecs")
set_kind("binary")
add_files("src/*.cpp", "tests/*.cpp")

add_includedirs("include")
