project = CreateProject()

project:CreateBinary("test_rbtree"):AddDependencies(
    project:CreateDependency()
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddSourceFiles("test_rbtree.c")
        :AddStaticLibrary("..", "utils_static"))

project:CreateBinary("test_str_utils"):AddDependencies(
    project:CreateDependency()
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddSourceFiles("test_str_utils.c")
        :AddStaticLibrary("..", "utils_static"))

project:CreateBinary("test_hash"):AddDependencies(
    project:CreateDependency()
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddSourceFiles("test_hash.c")
        :AddStaticLibrary("..", "utils_static"))

project:CreateBinary("test_bitmap"):AddDependencies(
    project:CreateDependency()
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddSourceFiles("test_bitmap.c")
        :AddStaticLibrary("..", "utils_static"))

project:CreateBinary("test_time_utils"):AddDependencies(
    project:CreateDependency()
        :AddFlags("-Wall", "-Werror", "-Wextra")
        :AddSourceFiles("test_time_utils.c")
        :AddStaticLibrary("..", "utils_static"))

return project
