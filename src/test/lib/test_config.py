import xmlrunner
import unittest
import tempfile
import dxrip

class TestConfig(unittest.TestCase):
    def setUp(self):
        pass

    def test_new(self):
        config = dxrip.lib.Config('test/fixture/lib/test_config')
        self.assertEqual('C:\Program Files\Vendor Name\Hoge.exe', config.target())

if __name__ == '__main__':
    unittest.main(testRunner = xmlrunner.XMLTestRunner(output = 'test/report'))
