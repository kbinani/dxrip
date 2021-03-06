import unittest
import dxrip
import xmlrunner
import os
import tempfile
import shutil

class TestInit(unittest.TestCase):
    def setUp(self):
        self.runner = dxrip.Init()
        self.work_directory = tempfile.mkdtemp()

    def tearDown(self):
        try:
            shutil.rmtree(self.work_directory)
        except Exception:
            pass

    def test_first_init_with_invalid_target_executable(self):
        try:
            self.runner.execute(["hogehoge"])
        except dxrip.Error as e:
            self.assertEqual(["error: file \"hogehoge\" does not exists"], e.messages)
        except Exception:
            self.fail("unexpected exception was raised")
        else:
            self.fail("expected exception was not raised")

    def test_execute(self):
        for dll_file in ["d3d9.dll", "d3d9callback.dll"]:
            source_file = os.path.join(os.path.dirname(dxrip.__file__), "data", dll_file)
            if not os.path.isfile(source_file):
                open(source_file, "w").close()

        status, work_file = tempfile.mkstemp(dir = self.work_directory)
        self.runner.execute([work_file])
        self.assertTrue(os.path.isdir(self.work_directory + '/.dxrip'))
        self.assertTrue(os.path.isfile(os.path.join(self.work_directory, '.dxrip', 'config')))
        self.assertTrue(os.path.isdir(os.path.join(self.work_directory, '.dxrip', 'mesh')))
        self.assertTrue(os.path.isdir(os.path.join(self.work_directory, '.dxrip', 'scene')))

    def test_execute_for_already_initialized(self):
        os.mkdir(self.work_directory + '/.dxrip')
        dummy_file = os.path.join(self.work_directory, 'dummy_file.exe')
        open(dummy_file, 'w').close()
        try:
            self.runner.execute([dummy_file])
        except dxrip.Error as e:
            self.assertEqual(['error: ".dxrip" directory already exists'], e.messages)
        else:
            self.fail("expected exception was not raised")

if __name__ == '__main__':
    unittest.main(testRunner = xmlrunner.XMLTestRunner(output = 'test/report'))
