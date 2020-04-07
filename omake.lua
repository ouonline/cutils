project = CreateProject()

target = project:CreateLibrary("utils", STATIC | SHARED)
target:AddSourceFiles("*.c")

return project
