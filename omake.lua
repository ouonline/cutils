project = CreateProject()

dep = project:CreateDependency()
dep:AddFlags("-Wall", "-Werror", "-Wextra", "-fPIC")
dep:AddSourceFiles("*.c")

a = project:CreateStaticLibrary("utils_static")
a:AddDependencies(dep)

so = project:CreateSharedLibrary("utils_shared")
so:AddDependencies(dep)

return project
