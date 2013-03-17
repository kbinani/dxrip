import argparse
import os
import dxrip
import shutil

class Init:
    __config = None

    def execute(self, arguments):
        self.__parse_arguments(arguments)
        self.__validate_options()
        self.__prepare_repository_directory()
        self.__deploy_dlls()

    def __parse_arguments(self, arguments):
        parser = argparse.ArgumentParser()
        parser.add_argument('target')
        args = parser.parse_args(arguments)
        self.__config = dxrip.lib.Config(os.path.dirname(args.target))
        self.__config.target = args.target

    def __validate_options(self):
        if self.__config.target is None:
            raise dxrip.Error([])
        if not os.path.isfile(self.__config.target):
            raise dxrip.Error(['error: file "' + self.__config.target + '" does not exists'])

    def __prepare_repository_directory(self):
        directory = os.path.dirname(self.__config.target)
        repository = os.path.join(directory, '.dxrip')
        if os.path.isdir(repository):
            raise dxrip.Error(['error: ".dxrip" directory already exists'])
        os.mkdir(repository)
        os.system("attrib +h \"" + repository + "\"")
        self.__config.write()

        os.mkdir(os.path.join(repository, 'mesh'))
        os.mkdir(os.path.join(repository, 'scene'))

    def __deploy_dlls(self):
        data_directory = os.path.join(os.path.dirname(dxrip.__file__), "data")
        destination_directory = os.path.dirname(self.__config.target)
        for file in ["d3d9.dll", "d3d9callback.dll"]:
            shutil.copyfile(os.path.join(data_directory, file), os.path.join(destination_directory, file))
