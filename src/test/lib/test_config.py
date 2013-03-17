import xmlrunner
import unittest
import tempfile
import dxrip
import os

class TestConfig(unittest.TestCase):
    def setUp(self):
        pass

    def test_new(self):
        config = dxrip.lib.Config("test/fixture/lib/test_config")
        self.assertEqual("Hoge Foo.exe", config.target)
        self.assertEqual(1, config.db_version)

    def test_write(self):
        directory = tempfile.mkdtemp()
        os.mkdir(os.path.join(directory, ".dxrip"))
        config = dxrip.lib.Config(directory)
        config.target = "Hoge.exe"
        config.db_version = 99
        config.write()

        conf_path = os.path.join(directory, ".dxrip", "config")
        self.assertTrue(os.path.isfile(conf_path))
        file = open(conf_path, "r")
        self.assertEqual(["[core]", "\ttarget = Hoge.exe", "\tdb_version = 99"], [line.rstrip() for line in file.readlines()])
        file.close()

    def test_get_db_file_path(self):
        config = dxrip.lib.Config("test/fixture/lib/test_config")
        actual = config.get_db_file_path()
        expected = "test/fixture/lib/test_config/.dxrip/db"
        self.assertEqual(os.path.normpath(expected), os.path.normpath(actual))

if __name__ == "__main__":
    unittest.main(testRunner = xmlrunner.XMLTestRunner(output = "test/report"))
