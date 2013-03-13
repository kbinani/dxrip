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
        self.assertEqual("C:\Program Files\Vendor Name\Hoge.exe", config.target)

    def test_write(self):
        directory = tempfile.mkdtemp()
        os.mkdir(os.path.join(directory, ".dxrip"))
        config = dxrip.lib.Config(directory)
        config.target = "Hoge.exe"
        config.write()

        conf_path = os.path.join(directory, ".dxrip", "config")
        self.assertTrue(os.path.isfile(conf_path))
        self.assertEqual(["[core]", "\ttarget = Hoge.exe"], [line.rstrip() for line in open(conf_path, "r").readlines()])

if __name__ == "__main__":
    unittest.main(testRunner = xmlrunner.XMLTestRunner(output = "test/report"))
