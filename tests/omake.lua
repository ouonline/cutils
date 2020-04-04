project = CreateProject()

test_rbtree = project:CreateBinary("test_rbtree")
test_rbtree:AddSourceFile("test_rbtree.c")
test_rbtree:AddStaticLibrary("..", "utils")

test_str_util = project:CreateBinary("test_str_util")
test_str_util:AddSourceFile("test_str_util.c")
test_str_util:AddStaticLibrary("..", "utils")

test_hash = project:CreateBinary("test_hash")
test_hash:AddSourceFile("test_hash.c")
test_hash:AddStaticLibrary("..", "utils")

test_bitmap = project:CreateBinary("test_bitmap")
test_bitmap:AddSourceFile("test_bitmap.c")
test_bitmap:AddStaticLibrary("..", "utils")

test_time_utils = project:CreateBinary("test_time_utils")
test_time_utils:AddSourceFile("test_time_utils.c")
test_time_utils:AddStaticLibrary("..", "utils")

return project
