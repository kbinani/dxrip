import dxrip
import sqlite3

class ObjectListDao:
    __connection = None
    __current_db_version = 1

    def __init__(self, config):
        self.__connection = sqlite3.connect(config.get_db_file_path())
        for migration in range(config.db_version + 1, self.__current_db_version + 1):
            method_name = "migration_" + str(migration)
            method = getattr(self, method_name)
            method(self.__connection)

    def __del__(self):
        self.__connection.close()

    def get_object_name_list(self):
        u"""Get object list.

        Returns:
            A list of object name
        """
        query = "SELECT `name` FROM `objects` ORDER BY `name` ASC;"
        return [record[0] for record in self.__connection.execute(query).fetchall()]

    def create_object(self, object_name):
        u"""Create new object with specified name.
        
        Args:
            object_name: Name of object. This should be uniq.

        Raises:
            dxrip.Error: An error occured 'object_name' duplication.

        """
        try:
            self.__connection.execute("INSERT INTO `objects`(`name`) VALUES(?)", (object_name, ))
            self.__connection.commit()
        except sqlite3.IntegrityError:
            self.__connection.rollback()
            raise dxrip.Error(["error: object name \"" + object_name + "\" is already in use."])

    def delete_object(self, object_name):
        u"""Delete object.
        
        Args:
            object_name: Name of object to be deleted.
        """
        pass

    def get_mesh_id_list(self, object_name):
        u"""Get list of mesh.
        
        Args:
            object_name: A name of object.

        Returns:
            A list of mesh_id.
        """
        pass

    def add_mesh_to(self, object_name, mesh_id):
        u"""Add mesh to specified object.
        
        Args:
            object_name: A name of object.
            mesh_id: A mesh_id to be added.
        """
        pass

    def remove_mesh(self, object_name, mesh_id):
        u"""Remove mesh from specified object.
        
        Args:
            object_name: A name of object.
            mesh_id: A mesh_id to be removed.
        """
        pass

    def migration_1(self, connection):
        connection.execute("CREATE TABLE `objects`(`id` INTEGER PRIMARY KEY, `name` TEXT NOT NULL);")
        connection.execute("CREATE UNIQUE INDEX `index_name` ON `objects`(`name`);")
        connection.execute("CREATE TABLE `meshes`(`id` TEXT PRIMARY KEY, `object_id` INTEGER NOT NULL);")
