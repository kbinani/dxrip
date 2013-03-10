import argparse
import os
import dxrip

class Init:
    def execute(self, arguments):
        parser = argparse.ArgumentParser()
        parser.add_argument('target')
        args = parser.parse_args(arguments)
        target = args.target
        if target is None:
            raise dxrip.Error([])
        if not os.path.isfile(target):
            raise dxrip.Error(["error: file \"hogehoge\" does not exists"])