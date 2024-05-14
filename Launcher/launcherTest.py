'''Form UI to import animations characters to the scene'''

import os
import sys
import json

from PySide6 import QtCore, QtGui, QtWidgets

DEFAULT_BUILD_ICON = "thumbnails\\default.jpg"
THUMBNAIL_PATH = "C:\\Users\\Jorge\\Desktop\\Launcher\\thumbnails"
THUMBNAIL_NAME = "thumbnail"
PROJECT_PATH = "projectPath"
PROJECT_DESCRTIPTION = "description"
PROJECT_GALLERY_PATH = "C:\\Users\\Jorge\\Desktop\\Launcher\\projectGallery.json"
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
            border-radius: 15px;
        }
        QLineEdit {
            border: 0px;
            background: rgba(30, 30, 30, 200);
            color: rgba(255, 116, 0, 180);
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
        print(path)


    def createNewProject(self):
        '''Create new project and add an entry for his settings in the project gallery'''
        # Create assets

        # Create hardcoded premake

        # Create projetSettings.lua

        # Create hardcoded GameApp code

        # Launch Editor with projectSettings.json
        

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
        self.correctThumbnailPath = False
        self.correctProjectPath = False
           
        self.setWindowFlags(QtCore.Qt.WindowType.FramelessWindowHint)
        self.setAttribute(QtCore.Qt.WidgetAttribute.WA_TranslucentBackground)
    
        self.centralWidget = QtWidgets.QWidget()

        # This container holds the window contents, so we can style it.
        self.centralWidget.setObjectName("Container")
        self.centralWidget.setStyleSheet(STYLESHEET_MAIN_WINDOW)

        self.titleBar = CustomTitleBar(self)

        # Widgets
        self.nameLabel = QtWidgets.QLineEdit(self,)
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
        self.thumbnailImage.setScaledContents(True)

        self.runButton = QtWidgets.QPushButton(self)
        self.runButton.setMaximumWidth(80)
        self.runButton.setText(windowTitle[:windowTitle.find(" ")])

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
        self.projectPathLabel.textEdited.connect(self.checkProjectPath)
        self.thumbnailNameLabel.textEdited.connect(self.checkThumbnailPath)
        self.runButton.clicked.connect(self.createProject)


    def createProject(self):
        """Create or load the project"""
        self.project.createNewProject()
        self.close()


    def checkProjectPath(self, path):
        """Check if the path exist to create projectSettings.lua"""
        if (os.path.exists(path) == False):
            self.correctProjectPath = False
            self.projectPathLabel.setStyleSheet("QLineEdit {color:red}")
        else:
            self.correctProjectPath = True
            self.projectPathLabel.setStyleSheet("QTextEdit {color:green}")
        
    def checkThumbnailPath(self, path):
        """Check if the thumbnail image is in the resources folder"""
     
        if (os.path.exists(os.path.join(THUMBNAIL_PATH, path)) == False):
            self.correctThumbnailPath = False
            self.thumbnailNameLabel.setStyleSheet("QLineEdit {color:red}")
            self.thumbnailImage.setPixmap(QtGui.QPixmap())
            self.thumbnailImage.setFixedSize(0, 0)
        else:
            self.correctThumbnailPath = True
            self.thumbnailNameLabel.setStyleSheet("QTextEdit {color:green}")
            self.thumbnailImage.setPixmap(QtGui.QPixmap(os.path.join(THUMBNAIL_PATH, path)))
            self.thumbnailImage.setFixedSize(THUMBNAIL_SIZE[0], THUMBNAIL_SIZE[1])

    
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
            if not os.path.exists(thumbnailPath):
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