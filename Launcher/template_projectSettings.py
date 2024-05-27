class ProjectSettingsTemplate():
    
    def __init__(self, name, assetsPath):
        print(assetsPath)
        self.template = """\
{{
    "name": "\\"{0}\\"",
    "assetsPath": "\\"{1}\\"",
    "buildPath": "%{{prj.name}}/build",
    "lastScene": "SceneDefault.lua",
    "screenWidth": 1280,
    "screenHeight": 720
}}""".format(name, assetsPath)