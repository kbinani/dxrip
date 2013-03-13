import configparser
import os

class Config:
    __directory = None
    __target = None

    def __init__(self, directory):
        self.__directory = directory
        self.__read(self.__directory)

    def target(self):
        return self.__target

    def __read(self, directory):
        conf = configparser.SafeConfigParser()
        conf_path = os.path.join(directory, '.dxrip', 'config')
        conf.read(conf_path)
        self.__target = conf.get('core', 'target')
