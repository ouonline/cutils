project = Project()

flags = {"-Wall", "-Werror", "-Wextra"}

project:CreateBinary("test_rbtree"):AddDependencies(
    project:CreateDependency()
        :AddFlags(flags)
        :AddSourceFiles("test_rbtree.c")
        :AddStaticLibraries("..", "utils_static"))

project:CreateBinary("test_str_utils"):AddDependencies(
    project:CreateDependency()
        :AddFlags(flags)
        :AddSourceFiles("test_str_utils.c")
        :AddStaticLibraries("..", "utils_static"))

project:CreateBinary("test_hash"):AddDependencies(
    project:CreateDependency()
        :AddFlags(flags)
        :AddSourceFiles("test_hash.c")
        :AddStaticLibraries("..", "utils_static"))

project:CreateBinary("test_bitmap"):AddDependencies(
    project:CreateDependency()
        :AddFlags(flags)
        :AddSourceFiles("test_bitmap.c")
        :AddStaticLibraries("..", "utils_static"))

project:CreateBinary("test_time_utils"):AddDependencies(
    project:CreateDependency()
        :AddFlags(flags)
        :AddSourceFiles("test_time_utils.c")
        :AddStaticLibraries("..", "utils_static"))

return project
