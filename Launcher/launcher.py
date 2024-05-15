'''Form UI to import animations characters to the scene'''

import os
import sys
import json
import subprocess

from PySide6 import QtCore, QtGui, QtWidgets

from template_gameApp import GameAppTemplate
from template_projectSettings import ProjectSettingsTemplate
from template_premake5 import Premake5Template
from template_sceneDefault import SceneDefaultTemplate


ASSETS_FOLDER = "assets"
AUDIO_FOLDER = "audio"
FILES_FOLDER = "files"
FONTS_FOLDER = "fonts"
IMAGES_FOLDER = "images"
MODELS_FOLDER = "models"
SCENES_FOLDER = "scenes"
SCRIPTS_FOLDER = "scripts"
SHADERS_FOLDER = "shaders"
ALL_ASSETS_FOLDERS = [AUDIO_FOLDER, FILES_FOLDER, FONTS_FOLDER, IMAGES_FOLDER,
                      MODELS_FOLDER, SCENES_FOLDER, SCRIPTS_FOLDER, SHADERS_FOLDER]

DEFAULT_BUILD_ICON = ".\\thumbnails\\default.jpg"
THUMBNAIL_PATH = ".\\thumbnails"
THUMBNAIL_NAME = "thumbnail"
PROJECT_PATH = "projectPath"
PROJECT_SETTINGS = "projectSettings.json"
PREMAKE5 = "premake5.lua"
SCENE_DEFAULT_PATH = "assets\\scenes\\SceneDefault.lua"
PROJECT_DESCRTIPTION = "description"
PROJECT_GALLERY_PATH = ".\\projectGallery.json"
THUMBNAIL_SIZE = (260, 200)

STYLESHEET_MAIN_WINDOW = """
        #Container {
            background: qlineargradient(x1:0 y1:0, x2:1 y2:1, stop:0 rgba(15, 15, 15, 235) stop:1 rgba(0, 0, 0, 235));
            border-radius: 20px;
            border-color: transparent;
        }"""

STYLESHEET = """
        QTextEdit {
            border: 0px;
            background: rgba(30, 30, 30, 200);
            color: rgba(255, 116, 0, 180);
            padding: 10px;
            margin-top:10px;
            border-radius: 15px;
        }
        QLineEdit {
            border: 0px;
            background: rgba(30, 30, 30, 200);
            color: rgba(255, 116, 0, 180);
            margin-top:10px;
            padding: 10px;
            border-radius: 15px;
        }
        QPushButton {
            border: 0px;
            background: rgba(30, 30, 30, 200);
            color: rgba(255, 116, 0, 180);
            padding: 10px;
            margin-top:20px;
            border-radius: 15px;
        }
        QPushButton:hover {
            background: rgba(255, 116, 0, 180);
            color: rgba(20, 20, 20, 150);
            font-weight: bold;
        }
        QListWidget {
            border-radius: 10px;
            background-color: transparent;
            color: rgb(200, 200, 200);
        }
        QListWidget:item {
            border-radius: 15px;
            color: rgb(200, 200, 200);
            background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,
                                stop: 0 rgba(0, 0, 0, 0), stop: 1 rgb(20, 20, 20));
            selection-color: rbga(100, 0, 0 , 255);
        }
        QListWidget:item:hover {
            padding: 10px;
            border: 1px solid rgba(255, 116, 0, 120);
            background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0,
                                stop: 0 rgba(0, 0, 0, 0), stop: 1 rgba(0, 0, 0, 200));
        }
        QListWidget:item:selected {
            background-color: rgb(0, 0, 0);
        }
        QScrollBar {
            width: 0px;
        }"""


class ProjectAPI():
    """Handle project management and settings"""
    
    def __init__(self):
        self.projectPath = PROJECT_GALLERY_PATH


    def loadProject(self, item):
        '''Load project from a project gallery item'''
        path = item.data(role=QtCore.Qt.ToolTipRole)
        self.load(path)


    def createNewProject(self, name, description, projectPath, thumbnail):
        '''Create new project and add an entry for his settings in the project gallery'''
        
        # Add project to projectGallery
        projectGallery = []
        if os.path.isfile(PROJECT_GALLERY_PATH) is False:
            raise Exception("File not found")
        
        with open(PROJECT_GALLERY_PATH) as fp:
            projectGallery = json.load(fp)

        projectGallery.update({ name: {
            PROJECT_PATH: os.path.normpath(projectPath),
            PROJECT_DESCRTIPTION: description,
            THUMBNAIL_NAME: thumbnail
        }})

        with open(PROJECT_GALLERY_PATH, 'w') as file:
            json.dump(projectGallery, file,  indent=4, separators=(',',': '))

        # Create assets folders
        if (len(os.listdir(projectPath)) > 0):
            raise Exception("Game folder is not empty")
        
        # Create projetSettings.lua
        file = open(os.path.join(projectPath, PROJECT_SETTINGS), 'w')
        projecSettinsTemplate = ProjectSettingsTemplate(name, projectPath)
        file.write(projecSettinsTemplate.template)
        file.close()
        
        os.mkdir(os.path.join(projectPath, ASSETS_FOLDER))
        for assetFolder in ALL_ASSETS_FOLDERS:
            os.mkdir(os.path.join(projectPath, ASSETS_FOLDER, assetFolder))

        # Create hardcoded premake
        file = open(os.path.join(projectPath, PREMAKE5), 'w')
        premakeTemplate = Premake5Template(name)
        file.write(premakeTemplate.template)
        file.close()

        # Create hardcoded scene default
        file = open(os.path.join(projectPath, SCENE_DEFAULT_PATH), 'w')
        sceneDefaultTemplate = SceneDefaultTemplate()
        file.write(sceneDefaultTemplate.template)
        file.close()

        # Create hardcoded GameApp code
        gameCodeTemplate = GameAppTemplate(name, projectPath)
        gameCodeTemplate.GenerateTemplateCodes()

        # Launch Editor with projectSettings.json
        self.load(projectPath)


    def load(self, path):
        # Read projectSettings from path and pass through command args

        if (os.path.exists(os.path.join(path, PROJECT_SETTINGS)) == False):
            print(f"There is not {PROJECT_SETTINGS} in the project path")
            return
        
        settings = []
        with open(os.path.join(path, PROJECT_SETTINGS)) as file:
            settings = json.load(file)
        
        subprocess.Popen(f"..\\bin\\Debug-windows-x86_64\\Editor\\Editor.exe \
            {settings["name"]} {settings["assetsPath"]} {settings["screenWidth"]} {settings["screenHeight"]}")
        return



class CustomTitleBar(QtWidgets.QWidget):
    def __init__(self, parent):
        super().__init__(parent)
        self.setBackgroundRole(QtGui.QPalette.ColorRole.Highlight)
        self.setMaximumHeight(35)

        self.initialPos = None
        titleBarLayout = QtWidgets.QHBoxLayout(self)
        titleBarLayout.setContentsMargins(1, 1, 1, 1)
        titleBarLayout.setSpacing(5)

        self.title = QtWidgets.QLabel(f"{self.__class__.__name__}", self)
        self.title.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
        self.title.setStyleSheet("""
            QLabel {
                text-transform: uppercase; 
                font: Consolas;
                font-size: 12pt; 
                margin-top: 5px;
                margin-left: 100px;
                color: rgba(255, 116, 0, 180);
            }
            """
        )
        if title := parent.windowTitle():
            self.title.setText(title)
        titleBarLayout.addWidget(self.title)

        # Min button
        self.minButton = QtWidgets.QToolButton(self)
        minIcon = self.style().standardIcon(QtWidgets.QStyle.StandardPixmap.SP_TitleBarMinButton)
        self.minButton.setIcon(minIcon)
        self.minButton.clicked.connect(self.window().showMinimized)

        # Max button
        self.maxButton = QtWidgets.QToolButton(self)
        maxIcon = self.style().standardIcon(QtWidgets.QStyle.StandardPixmap.SP_TitleBarMaxButton)
        self.maxButton.setIcon(maxIcon)
        self.maxButton.clicked.connect(self.window().showMaximized)

        # Close button
        self.closeButton = QtWidgets.QToolButton(self)
        closeIcon = self.style().standardIcon(QtWidgets.QStyle.StandardPixmap.SP_TitleBarCloseButton)
        self.closeButton.setIcon(closeIcon)
        self.closeButton.clicked.connect(self.window().close)

        # Normal button
        self.normalButton = QtWidgets.QToolButton(self)
        normalIcon = self.style().standardIcon(QtWidgets.QStyle.StandardPixmap.SP_TitleBarNormalButton)
        self.normalButton.setIcon(normalIcon)
        self.normalButton.clicked.connect(self.window().showNormal)
        self.normalButton.setVisible(False)
    
        buttons = [
            self.minButton,
            self.normalButton,
            self.maxButton,
            self.closeButton,
        ]
        for button in buttons:
            button.setFocusPolicy(QtCore.Qt.FocusPolicy.NoFocus)
            button.setFixedSize(QtCore.QSize(20, 20))
            button.setStyleSheet("""
                 QToolButton {
                    border: none;
                    padding: 2px;
                }""")
            titleBarLayout.addWidget(button)

    def windowStateChanged(self, state):
        if state == QtCore.Qt.WindowState.WindowMaximized:
            self.normalButton.setVisible(True)
            self.maxButton.setVisible(False)
        else:
            self.normalButton.setVisible(False)
            self.maxButton.setVisible(True)

    def changeEvent(self, event):
        if event.type() == QtCore.QEvent.Type.WindowStateChange:
            self.titleBar.windowStateChanged(self.windowState())
        super().changeEvent(event)
        event.accept()
        
    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.MouseButton.LeftButton:
            self.initialPos = event.position().toPoint()
        super().mousePressEvent(event)
        event.accept()

    def mouseMoveEvent(self, event):
        if self.initialPos is not None:
            delta = event.position().toPoint() - self.initialPos
            self.window().move(
                self.window().x() + delta.x(), 
                self.window().y() + delta.y(),
            )
        super().mouseMoveEvent(event)
        event.accept()

    def mouseReleaseEvent(self, event):
        self.initialPos = None
        super().mouseReleaseEvent(event)
        event.accept()


class CreateProjectWindow(QtWidgets.QMainWindow):
    def __init__(self, windowTitle):
        super().__init__()

        self.setWindowTitle(windowTitle)

        self.project = ProjectAPI()
        self.titleBar = CustomTitleBar(self)
        self.correctProjectPath = False
        self.correctProjectName = False
           
        self.setWindowFlags(QtCore.Qt.WindowType.FramelessWindowHint)
        self.setAttribute(QtCore.Qt.WidgetAttribute.WA_TranslucentBackground)
    
        self.centralWidget = QtWidgets.QWidget()

        # This container holds the window contents, so we can style it.
        self.centralWidget.setObjectName("Container")
        self.centralWidget.setStyleSheet(STYLESHEET_MAIN_WINDOW)

        self.titleBar = CustomTitleBar(self)

        # Widgets
        self.nameLabel = QtWidgets.QLineEdit(self)
        self.nameLabel.setPlaceholderText("Name")

        self.descriptionLabel = QtWidgets.QTextEdit(self)
        self.descriptionLabel.setPlaceholderText("Description")
        self.descriptionLabel.setMinimumHeight(100)
        self.descriptionLabel.setAlignment(QtCore.Qt.AlignmentFlag.AlignTop)

        self.projectPathLabel = QtWidgets.QLineEdit(self)
        self.projectPathLabel.setPlaceholderText("Project Path")

        self.thumbnailNameLabel = QtWidgets.QLineEdit(self)
        self.thumbnailNameLabel.setPlaceholderText("Thumbnail name")

        self.thumbnailImage = QtWidgets.QLabel(self)
        self.thumbnailImage.setFixedSize(THUMBNAIL_SIZE[0], THUMBNAIL_SIZE[1])
        self.thumbnailImage.setScaledContents(True)
        self.thumbnailImage.setFixedSize(0, 0)

        self.runButton = QtWidgets.QPushButton(self)
        self.runButton.setText(windowTitle[:windowTitle.find(" ")].upper())
        self.runButton.setMinimumWidth(65)

        # Layouts
        self.formLayout = QtWidgets.QVBoxLayout()
        self.formLayout.setContentsMargins(11, 11, 11, 11)
        self.formLayout.addWidget(self.nameLabel)
        self.formLayout.addWidget(self.descriptionLabel)
        self.formLayout.addWidget(self.projectPathLabel)
        self.formLayout.addWidget(self.thumbnailNameLabel)

        self.imageLayout = QtWidgets.QVBoxLayout()
        self.imageLayout.addWidget(self.thumbnailImage)
        self.imageLayout.setAlignment(QtCore.Qt.AlignmentFlag.AlignHCenter)


        self.runLayout = QtWidgets.QHBoxLayout()
        self.runLayout.addWidget(self.runButton)
        self.runLayout.setAlignment(QtCore.Qt.AlignmentFlag.AlignHCenter)

        self.mainLayout = QtWidgets.QVBoxLayout()
        self.mainLayout.setAlignment(QtCore.Qt.AlignmentFlag.AlignTop)
        self.mainLayout.addWidget(self.titleBar)
        self.mainLayout.addLayout(self.formLayout)
        self.mainLayout.addLayout(self.imageLayout)
        self.mainLayout.addLayout(self.runLayout)

        self.centralWidget.setLayout(self.mainLayout)
        self.setCentralWidget(self.centralWidget)

        # Connections
        self.nameLabel.textEdited.connect(self.checkProjectName)
        self.projectPathLabel.textEdited.connect(self.checkProjectPath)
        self.thumbnailNameLabel.textEdited.connect(self.checkThumbnailPath)
        self.runButton.clicked.connect(self.createProject)


    def checkProjectName(self):
        """Check if the name of the project is not repeated"""
        projectGallery = []
        if os.path.isfile(PROJECT_GALLERY_PATH) is False:
            raise Exception("File not found")
        
        with open(PROJECT_GALLERY_PATH) as file:
            projectGallery = json.load(file)

        if (self.nameLabel.text() in projectGallery.keys()):
            self.correctProjectName = False
            self.nameLabel.setStyleSheet("QLineEdit {color:red}")
        else:
            self.correctProjectName = True
            self.nameLabel.setStyleSheet("QLineEdit {color:green}")
    

    def createProject(self):
        """Create or load the project"""
        name = self.nameLabel.text()
        description = self.descriptionLabel.toPlainText()
        thumbnail = self.thumbnailNameLabel.text()
        projectPath = self.projectPathLabel.text()

        if (self.correctProjectPath and self.correctProjectName and name != "" and description != ""):
            self.project.createNewProject(name, description, projectPath , thumbnail)
            self.close()


    def checkProjectPath(self, path):
        """Check if the path exist to create projectSettings.lua"""
        if (os.path.exists(path) == False):
            self.correctProjectPath = False
            self.projectPathLabel.setStyleSheet("QLineEdit {color:red}")
        elif (os.path.exists(path) == True and path != ""):
            self.projectPathLabel.setStyleSheet("QLineEdit {color:green}")
            self.correctProjectPath = True
        
    def checkThumbnailPath(self, path):
        """Check if the thumbnail image is in the resources folder"""
        if (os.path.exists(os.path.join(THUMBNAIL_PATH, path)) == False):
            self.thumbnailNameLabel.setStyleSheet("QLineEdit {color:red}")
            self.thumbnailImage.setFixedSize(0, 0)
        elif (os.path.exists(os.path.join(THUMBNAIL_PATH, path)) == True and path != ""):
            self.thumbnailNameLabel.setStyleSheet("QLineEdit {color:green}")
            self.thumbnailImage.setPixmap(QtGui.QPixmap(os.path.join(THUMBNAIL_PATH, path)))
            self.thumbnailImage.setFixedSize(THUMBNAIL_SIZE[0],THUMBNAIL_SIZE[1])

    
class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle(f"Cober Engine Launcher")

        self.titleBar = CustomTitleBar(self)
        self.setMinimumHeight(800)
        self.setMinimumWidth(600)
           
        self.setWindowFlags(QtCore.Qt.WindowType.FramelessWindowHint)
        self.setAttribute(QtCore.Qt.WidgetAttribute.WA_TranslucentBackground)
    
        self.centralWidget = QtWidgets.QWidget()

        # This container holds the window contents, so we can style it.
        self.centralWidget.setObjectName("Container")
        self.centralWidget.setStyleSheet(STYLESHEET_MAIN_WINDOW)

        self.titleBar = CustomTitleBar(self)

        self.launchWidgetLayout = QtWidgets.QVBoxLayout()
        self.launchWidgetLayout.setContentsMargins(11, 11, 11, 11)
        self.launchWidgetLayout.addWidget(LauncherWindow())

        self.mainLayout = QtWidgets.QVBoxLayout()
        # self.mainLayout.setContentsMargins(0, 0, 0, 0)
        self.mainLayout.setAlignment(QtCore.Qt.AlignmentFlag.AlignTop)
        self.mainLayout.addWidget(self.titleBar)
        self.mainLayout.addLayout(self.launchWidgetLayout)

        self.centralWidget.setLayout(self.mainLayout)
        self.setCentralWidget(self.centralWidget)
    
    
    def changeEvent(self, event):
        if event.type() == QtCore.QEvent.Type.WindowStateChange:
            self.titleBar.windowStateChanged(self.windowState())
        super().changeEvent(event)
        event.accept()
        
    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.MouseButton.LeftButton:
            self.initialPos = event.position().toPoint()
        super().mousePressEvent(event)
        event.accept()

    def mouseMoveEvent(self, event):
        if self.initialPos is not None:
            delta = event.position().toPoint() - self.initialPos
            self.window().move(
                self.window().x() + delta.x(), 
                self.window().y() + delta.y(),
            )
        super().mouseMoveEvent(event)
        event.accept()

    def mouseReleaseEvent(self, event):
        self.initialPos = None
        super().mouseReleaseEvent(event)
        event.accept()


class LauncherWindow(QtWidgets.QDialog):
    '''Launch projects main dialog. Show gallery and options'''

    def __init__(self, parent=None):
        super(LauncherWindow, self).__init__(parent)

        QtWidgets.QApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        self.defaultThumbnail = QtGui.QIcon(DEFAULT_BUILD_ICON)

        self.setContentsMargins(11, 11, 11, 11)
  
        # Widgets
        self.createProjectWindow = None

        self.createProjectButton = QtWidgets.QPushButton("CREATE NEW PROJECT")
        self.createProjectButton.setFont(QtGui.QFont("Consolas", 11))
        self.createProjectButton.setMaximumWidth(400)
        self.createProjectButton.setFocusPolicy(QtCore.Qt.NoFocus)

        self.projectList = QtWidgets.QListWidget(self)
        self.projectList.setSpacing(10)
        self.projectList.setFont(QtGui.QFont("Consolas", 10))
        self.projectList.setViewMode(QtWidgets.QListView.ListMode)
        self.projectList.setResizeMode(QtWidgets.QListView.Adjust)
        self.projectList.setAcceptDrops(False)
        self.projectList.setMovement(QtWidgets.QListView.Static)
        self.projectList.setFocusPolicy(QtCore.Qt.NoFocus)
        self.projectList.setVerticalScrollMode(QtWidgets.QAbstractItemView.ScrollMode.ScrollPerPixel)
        self.projectList.resetHorizontalScrollMode()
        self.projectList.setUniformItemSizes(True);

        # Layouts
        self.mainLayout = QtWidgets.QVBoxLayout(self)
        self.mainLayout.addWidget(self.projectList)

        self.runButtonsLayout = QtWidgets.QHBoxLayout(self)
        self.runButtonsLayout.setAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
        self.runButtonsLayout.addWidget(self.createProjectButton)
        self.mainLayout.addLayout(self.runButtonsLayout)

        # Connections
        self.projectList.doubleClicked.connect(self.loadProject)
        self.createProjectButton.clicked.connect(self.createNewProject)

        self.generateProjectsList()

        QtWidgets.QApplication.restoreOverrideCursor()

    def loadProject(self, item):
        if (self.createProjectWindow):
            self.createProjectWindow.close()
        project = ProjectAPI()
        project.loadProject(item)
        self.window().close()
    
    def createNewProject(self):
        self.createProjectWindow = CreateProjectWindow("Create New Project")
        self.createProjectWindow.setStyleSheet(STYLESHEET)
        self.createProjectWindow.show()

    def resizeEvent(self, event):
        constant = 50
        newSize = QtCore.QSize(event.size().width()/2 - constant-20, event.size().height() - constant)
        self.projectList.setIconSize(newSize)

    def generateProjectsList(self):
        '''Generate projects gallery'''
        data = open(PROJECT_GALLERY_PATH)
        self.assets = json.load(data)
        data.close()

        for project, projectSettings in self.assets.items():

            thumbnail = QtWidgets.QListWidgetItem(project + "\n\n" + projectSettings[PROJECT_DESCRTIPTION])
            thumbnail.setToolTip(projectSettings[PROJECT_PATH])

            thumbnailPath = os.path.join(THUMBNAIL_PATH, projectSettings[THUMBNAIL_NAME])
            if not os.path.exists(thumbnailPath) or projectSettings[THUMBNAIL_NAME] == "":
                thumbnailPath = self.defaultThumbnail

            thumbnail.setIcon(QtGui.QIcon(thumbnailPath))
            thumbnail.setTextAlignment(QtCore.Qt.AlignmentFlag.AlignLeft | QtCore.Qt.AlignmentFlag.AlignCenter)

            self.projectList.addItem(thumbnail)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    
    palette = QtGui.QPalette()
    palette.setColor(QtGui.QPalette.ColorRole.Highlight, QtGui.QColor(255, 116, 0, 255))
    app.setPalette(palette)

    window = MainWindow()
    window.setStyleSheet(STYLESHEET)

    window.show()

    app.exec()
