class ProjectSettingsTemplate():
    
    def __init__(self, name, assetsPath):
        print(assetsPath)
        self.template = """\
local projectSettings =
{{
    name = "{0}",
    assetsPath = "{1}",
    buildPath = "%{{prj.name/build}}",
    screenWidth = 1280,
    screenHeight = 720
}}
return projectSettings""".format(name, assetsPath)