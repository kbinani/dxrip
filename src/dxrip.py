import sys
from error import *
from init import *
from scene import *
import lib

if __name__ == "__main__":
    sub_command = sys.argv[1] if len(sys.argv) >= 2 else "help"
    sub_arguments = sys.argv[2:]
    runner = None

    if sub_command == "init":
        runner = Init()
    elif sub_command == "scene":
        runner = Scene()
    elif sub_command == "help":
        print("help")
    else:
        print("ERROR: unknown command")
        exit(1)

    try:
        runner.execute(sub_arguments)
    except Error as e:
        for message in e.messages:
            print(message)
        exit(1)
