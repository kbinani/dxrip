import dxrip
import os
import argparse
import tempfile
import shutil

class Scene:
    def execute(self, arguments):
        config = dxrip.lib.Config(os.getcwd())
        scene = self.__get_scene_number(arguments)
        dao = dxrip.lib.db.SceneObjectDao(config)
        x_file_list = self.__get_x_file_list(dao, scene, config.get_mesh_directory_path())
        temporary_script_path = self.__create_temporary_script(x_file_list)
        self.__execute_temporary_script(config.blender, temporary_script_path)

    def __get_scene_number(self, arguments):
        parser = argparse.ArgumentParser()
        parser.add_argument('scene')
        args = parser.parse_args(arguments)
        return int(args.scene)

    def __get_x_file_list(self, dao, scene, mesh_directory_path):
        object_list = dao.get_object_list(scene)
        if len(object_list) == 0:
            raise dxrip.Error(["error: scene " + str(scene) + " does not exist"])

        return [os.path.join(mesh_directory_path, object_id + ".x").replace("\\", "\\\\") for object_id in object_list]

    def __create_temporary_script(self, x_file_list):
        file_list = "file_list = [\"" + "\", \"".join(x_file_list) + "\"]"

        data_directory = os.path.join(os.path.dirname(dxrip.__file__), "data")
        template_path = os.path.join(data_directory, "ximporter.py.template")
        with open(template_path, "r") as template_file:
            contents = template_file.read().replace("#<file_list>#", file_list)
        fd, outfile_path = tempfile.mkstemp()
        with os.fdopen(fd, "w") as outfile:
            outfile.write(contents)
        return outfile_path

    def __execute_temporary_script(self, blender, temporary_script_path):
        data_directory = os.path.join(os.path.dirname(dxrip.__file__), "data")
        fd, empty_blend_path = tempfile.mkstemp()
        os.fdopen(fd, "w").close()
        os.remove(empty_blend_path)
        shutil.copyfile(os.path.join(data_directory, "empty.blend"), empty_blend_path)

        command = '"' + blender + '" ' + empty_blend_path + ' --python ' + temporary_script_path
        os.system(command)
