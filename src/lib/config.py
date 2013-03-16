import configparser
import os

class Config:
    __directory = None
    target = None
    db_version = 0
    #TODO(kbinani): this can be modified by; dxrip config core.blender "C:\Program Files\Blender Foundation\Blender\blender.exe"
    blender = "C:\\Program Files\\Blender Foundation\\Blender\\blender.exe"

    def __init__(self, directory):
        self.__directory = directory
        conf_path = self.__get_conf_path()
        if os.path.isfile(conf_path):
            self.__read(conf_path)

    def write(self):
        conf_path = self.__get_conf_path()
        contents = ["[core]",
                    "\ttarget = " + self.target,
                    "\tdb_version = " + str(self.db_version)]
        file = open(conf_path, "w")
        file.writelines([line + "\n" for line in contents])
        file.close()

    def get_db_file_path(self):
        return os.path.join(self.__directory, ".dxrip", "db")

    def get_scene_directory_path(self):
        return os.path.join(self.__directory, ".dxrip", "scene")

    def __read(self, conf_path):
        conf = configparser.ConfigParser()
        conf.read(conf_path)
        self.target = conf.get("core", "target")
        self.db_version = int(conf.get("core", "db_version"))

    def __get_conf_path(self):
        return os.path.join(self.__directory, ".dxrip", "config")
