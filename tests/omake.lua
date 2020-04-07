project = CreateProject()

test_rbtree = project:CreateBinary("test_rbtree")
test_rbtree:AddSourceFiles("test_rbtree.c")
test_rbtree:AddLibrary("..", "utils", STATIC)

test_str_util = project:CreateBinary("test_str_util")
test_str_util:AddSourceFiles("test_str_util.c")
test_str_util:AddLibrary("..", "utils", STATIC)

test_hash = project:CreateBinary("test_hash")
test_hash:AddSourceFiles("test_hash.c")
test_hash:AddLibrary("..", "utils", STATIC)

test_bitmap = project:CreateBinary("test_bitmap")
test_bitmap:AddSourceFiles("test_bitmap.c")
test_bitmap:AddLibrary("..", "utils", STATIC)

test_time_utils = project:CreateBinary("test_time_utils")
test_time_utils:AddSourceFiles("test_time_utils.c")
test_time_utils:AddLibrary("..", "utils", STATIC)

return project
