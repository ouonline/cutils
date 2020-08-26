project = Project()

dep = project:CreateDependency()
    :AddFlags({"-Wall", "-Werror", "-Wextra", "-fPIC"})
    :AddSourceFiles("*.c")

project:CreateStaticLibrary("utils_static"):AddDependencies(dep)
project:CreateSharedLibrary("utils_shared"):AddDependencies(dep)

return project
