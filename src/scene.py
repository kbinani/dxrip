import dxrip
import os
import argparse
import tempfile
import shutil

class Scene:

    def execute(self, arguments):
        config = dxrip.lib.Config(os.getcwd())

        parser = argparse.ArgumentParser()
        parser.add_argument('scene')
        args = parser.parse_args(arguments)
        scene = int(args.scene)

        scene_directory = os.path.join(config.get_scene_directory_path(), str(scene))
        if not os.path.isdir(scene_directory):
            raise dxrip.Error(["error: scene " + str(scene) + " does not exist"])

        all_file_list = [os.path.join(scene_directory, file) for file in os.listdir(scene_directory)]
        x_file_list = [file.replace("\\", "\\\\") for file in all_file_list if os.path.isfile(file) and os.path.splitext(file)[1] == ".x"]

        data_directory = os.path.join(os.path.dirname(dxrip.__file__), "data")
        template_path = os.path.join(data_directory, "ximporter.py.template")
        template_file = open(template_path, "r")
        template_contents = template_file.read()
        template_file.close()
        file_list = "file_list = [\"" + "\", \"".join(x_file_list) + "\"]"
        contents = template_contents.replace("#<file_list>#", file_list)
        status, temporary_script_path = tempfile.mkstemp()

        temporary_script_file = open(temporary_script_path, "w")
        temporary_script_file.write(contents)
        temporary_script_file.close()

        fd, empty_blend_path = tempfile.mkstemp()
        os.fdopen(fd, "w").close()
        os.remove(empty_blend_path)
        shutil.copyfile(os.path.join(data_directory, "empty.blend"), empty_blend_path)

        command = '"' + config.blender + '" ' + empty_blend_path + ' --python ' + temporary_script_path
        os.system(command)
