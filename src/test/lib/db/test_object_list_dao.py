import unittest
import dxrip
import xmlrunner
import sqlite3
import os
import tempfile

class TestObjectListDao(unittest.TestCase):
    def test_init(self):
        test_repository = self.__create_test_repository()
        config = dxrip.lib.Config(test_repository)
        dao = dxrip.lib.db.ObjectListDao(config)
        db_file = os.path.join(test_repository, ".dxrip", "db")
        connection = sqlite3.connect(db_file)
        table_info = connection.execute("PRAGMA TABLE_INFO(`objects`)").fetchall()
        self.assertEqual(2, len(table_info))
        self.assertEqual("id", table_info[0][1])
        self.assertEqual("INTEGER", table_info[0][2])
        self.assertEqual("name", table_info[1][1])
        self.assertEqual("TEXT", table_info[1][2])

    def __create_test_repository(self, fixture_path = None):
        test_repository = tempfile.mkdtemp()
        os.mkdir(os.path.join(test_repository, ".dxrip"))
        if not fixture_path is None:
            fixture_db_file = os.path.join(test_repository, ".dxrip", "db")
            connection = sqlite3.connect(fixture_db_file)
            file = open(fixture_path, "r")
            for sql in file.readlines():
                connection.execute(sql)
            file.close()
            connection.close()
        return test_repository

if __name__ == "__main__":
    unittest.main(testRunner = xmlrunner.XMLTestRunner(output = "test/report"))
