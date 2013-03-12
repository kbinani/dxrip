import unittest
import dxrip
import xmlrunner

class TestInit(unittest.TestCase):
    def setUp(self):
        self.runner = dxrip.Init()

    def test_first_init_with_invalid_target_executable(self):
        try:
            self.runner.execute(["hogehoge"])
        except dxrip.Error as e:
            self.assertEqual(["error: file \"hogehoge\" does not exists"], e.messages)
        except Exception:
            self.fail("unexpected exception was raised")
        else:
            self.fail("expected exception was not raised")

if __name__ == '__main__':
    unittest.main(testRunner = xmlrunner.XMLTestRunner(output = 'test/report'))
