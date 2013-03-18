# dxrip
dxrip is a command line tool to rip off mesh and texture data from DirectX application.

# requirements
* Windows OS
* python (3.3 or later)
* Blender (2.66 or later)

# subcommands
## init
Initialize mesh and texture data repository for specified application executable.
### synopsis
    dxrip init [target]
### options
N/A
### example

    >C:
    >cd Program Files\Foo              <1>
    >dir /b                            <2>
    Data
    Settings
    Foo_Client.exe
    Updater.exe
    >dxrip init Foo_Client.exe         <3>
    >dir /b
    .dxrip                             <4>
    Data
    Settings
    d3d9.dll                           <4>
    d3d9callback.dll                   <4>
    Foo_Client.exe
    Updater.exe
1. ``cd`` to application directory
2. shows target application directory
3. execute ``init`` subcommand
4. created directory and files: these are managed by dxrip

## scene
Show objects in a scene using Blender.
### synopsis
    dxrip scene [scene_number]
### options
N/A

#license
GNU General Public License version 2

#note
* io_import_directx_x.py is Copyright © 2012 T.Yonemori
    * distributed under GPLv2 License at http://blender.jp/modules/newbb/viewtopic.php?post_id=7260&topic_id=1401&forum=5
* BaseCode, D3D9Interceptor is both Copyright © 2012 Matthew Fisher
    * distributed under MIT License at http://graphics.stanford.edu/~mdfisher/projects.html
