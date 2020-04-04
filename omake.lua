project = CreateProject()

target = project:CreateLibrary("utils")
target:AddSourceFile("*.c")

return project
