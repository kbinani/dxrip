import sqlite3

class SceneObjectDao:
    __connection = None

    def __init__(self, config):
        self.__connection = sqlite3.connect(config.get_scene_db_file_path())
        #TODO(kbinani): db migration

    def __del__(self):
        self.__connection.close();

    def get_object_list(self, scene_number):
        query = "SELECT `object_id` FROM `scene_objects` WHERE `scene` = " + str(scene_number) + ";"
        return [record[0] for record in self.__connection.execute(query).fetchall()]
