project = CreateProject()

flags = "-Wall -Werror -Wextra -fPIC"

test_rbtree = project:CreateBinary("test_rbtree")
test_rbtree:AddFlags(flags)
test_rbtree:AddSourceFiles("test_rbtree.c")
test_rbtree:AddStaticLibrary("..", "utils_static")

test_str_util = project:CreateBinary("test_str_util")
test_str_util:AddFlags(flags)
test_str_util:AddSourceFiles("test_str_util.c")
test_str_util:AddStaticLibrary("..", "utils_static")

test_hash = project:CreateBinary("test_hash")
test_hash:AddFlags(flags)
test_hash:AddSourceFiles("test_hash.c")
test_hash:AddStaticLibrary("..", "utils_static")

test_bitmap = project:CreateBinary("test_bitmap")
test_bitmap:AddFlags(flags)
test_bitmap:AddSourceFiles("test_bitmap.c")
test_bitmap:AddStaticLibrary("..", "utils_static")

test_time_utils = project:CreateBinary("test_time_utils")
test_time_utils:AddFlags(flags)
test_time_utils:AddSourceFiles("test_time_utils.c")
test_time_utils:AddStaticLibrary("..", "utils_static")

return project
