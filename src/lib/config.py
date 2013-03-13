import configparser
import os

class Config:
    __directory = None
    __target = None

    def __init__(self, directory):
        self.__directory = directory
        conf_path = self.__get_conf_path()
        if os.path.isfile(conf_path):
            self.__read(conf_path)

    def target(self):
        return self.__target

    def set_target(self, target):
        self.__target = target

    def write(self):
        conf_path = self.__get_conf_path()
        contents = ["[core]", "\ttarget = " + self.__target]
        file = open(conf_path, "w")
        file.writelines([line + "\n" for line in contents])
        file.close()

    def __read(self, conf_path):
        conf = configparser.SafeConfigParser()
        conf.read(conf_path)
        self.__target = conf.get("core", "target")

    def __get_conf_path(self):
        return os.path.join(self.__directory, ".dxrip", "config")
