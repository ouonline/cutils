project = CreateProject()

dep1 = project:CreateDependency()
dep1:AddFlags("-Wall", "-Werror", "-Wextra", "-fPIC")
dep1:AddSourceFiles("test_rbtree.c")
dep1:AddStaticLibrary("..", "utils_static")
test_rbtree = project:CreateBinary("test_rbtree")
test_rbtree:AddDependencies(dep1)

dep2 = project:CreateDependency()
dep2:AddFlags("-Wall", "-Werror", "-Wextra", "-fPIC")
dep2:AddSourceFiles("test_str_utils.c")
dep2:AddStaticLibrary("..", "utils_static")
test_str_utils = project:CreateBinary("test_str_utils")
test_str_utils:AddDependencies(dep2)

dep3 = project:CreateDependency()
dep3:AddFlags("-Wall", "-Werror", "-Wextra", "-fPIC")
dep3:AddSourceFiles("test_hash.c")
dep3:AddStaticLibrary("..", "utils_static")
test_hash = project:CreateBinary("test_hash")
test_hash:AddDependencies(dep3)

dep4 = project:CreateDependency()
dep4:AddFlags("-Wall", "-Werror", "-Wextra", "-fPIC")
dep4:AddSourceFiles("test_bitmap.c")
dep4:AddStaticLibrary("..", "utils_static")
test_bitmap = project:CreateBinary("test_bitmap")
test_bitmap:AddDependencies(dep4)

dep5 = project:CreateDependency()
dep5:AddFlags("-Wall", "-Werror", "-Wextra", "-fPIC")
dep5:AddSourceFiles("test_time_utils.c")
dep5:AddStaticLibrary("..", "utils_static")
test_time_utils = project:CreateBinary("test_time_utils")
test_time_utils:AddDependencies(dep5)

return project
