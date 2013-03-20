# Workflow

Initialize data repository by:

    $ dxrip init Foo_Client.exe

After running `Foo_Client.exe` to play the game. Then terminate `Foo_Client.exe`.

Displays captured scene list by:

    $ dxrip list scenes
    ---------------------------
    id  time
    ---------------------------
    1   2013-03-18 12:54:01.121
    2   2013-03-18 12:54:01.138
    3   2013-03-18 12:54:01.154

Displays the list of ripped objects by:

    $ dxrip list objects

List is now empty.
Determine scene 2 objects by:

    $ dxrip scene 2

Blender opens to show objects in scene #2.
Object hierarychy is like this:

    +Scene
        +Frame_da39a3ee5e6b4b0d3255bfef95601890afd80709
        +Frame_8df7f638da50ddfa8f6a4162ddfc738b65e8b1cf
        +KnownObjects

Then exit Blender.
Rip off "Wood" object from scene 2.
Edit scene using Blender, to delete objects which should not be contained to "Wood" object.
In this example, I want to rip off `Frame_da39a3ee5e6b4b0d3255bfef95601890afd80709` into "Wood", so delete `Frame_8df7f638da50ddfa8f6a4162ddfc738b65e8b1cf`.

    $ dxrip rip "Wood" --from 2

Save scene and exit Blender.
Determine ripped object "Wood" by:

    $ dxrip object "Wood"

Then exit Blender.
Confirm the list of ripped objects by:

    $ dxrip list objects
    Wood

Again, determine scene 2 objects using Blender by:

    $ dxrip scene 2

At this time, object hierarychy is like this:

    +Scene
        +Frame_8df7f638da50ddfa8f6a4162ddfc738b65e8b1cf
        +KnownObjects
            +Wood
                +Frame_da39a3ee5e6b4b0d3255bfef95601890afd80709

Determine information about "Wood" object by:

    $ dxrip info Wood
    name: Wood
    mesh: contains 1 mesh(es)
        #1 da39a3ee5e6b4b0d3255bfef95601890afd80709
