/*
Engine.cpp
Written by Matthew Fisher

See Engine.h for a defintion of all files included here.
*/

//All source files include Main.h
#include "Main.h"

#include "common\Engine\Core\Stdhdr.cpp"
#include "common\Engine\Core\UnicodeString.cpp"
#include "common\Engine\Core\String.cpp"
#include "common\Engine\Core\OutputDataStream.cpp"
#include "common\Engine\Core\InputDataStream.cpp"
#include "common\Engine\Core\Compression.cpp"

#include "common\Engine\Networking\Pipe.cpp"

#include "common\Engine\Math\RGBColor.cpp"
#include "common\Engine\Math\SpaceVector.cpp"
#include "common\Engine\Math\Line3D.cpp"
#include "common\Engine\Math\LineSegment2D.cpp"
#include "common\Engine\Math\LineSegment3D.cpp"
#include "common\Engine\Math\Plane.cpp"
#include "common\Engine\Math\Intersect.cpp"
#include "common\Engine\Math\Matrix4.cpp"
#include "common\Engine\Math\Distance.cpp"

#include "common\Engine\Multithreading\Thread.cpp"

#include "common\Engine\Audio\AudioDevice.cpp"
#include "common\Engine\Audio\WaveFile.cpp"
#include "common\Engine\Audio\WaveFileReader.cpp"

#include "common\Engine\Graphics Objects\Bitmap.cpp"
#include "common\Engine\Graphics Objects\MatrixController.cpp"
//#include "common\Engine\Graphics Objects\Camera.cpp"
#include "common\Engine\Graphics Objects\PrimitiveRender.cpp"
#include "common\Engine\Graphics Objects\MeshVertex.cpp"
#include "common\Engine\Graphics Objects\KDTree3.cpp"
#include "common\Engine\Graphics Objects\KDTreeN.cpp"
#include "common\Engine\Graphics Objects\PointSet.cpp"
#include "common\Engine\Graphics Objects\RayIntersectorBruteForce.cpp"
#include "common\Engine\Graphics Objects\RayIntersectorKDTree.cpp"

#include "common\Engine\Simple Mesh\BaseMesh.cpp"
#include "common\Engine\Simple Mesh\D3D9Mesh.cpp"
#include "common\Engine\Simple Mesh\D3D10Mesh.cpp"
#include "common\Engine\Simple Mesh\Mesh.cpp"

#include "common\Engine\Complex Mesh\Triangle.cpp"
#include "common\Engine\Complex Mesh\FullEdge.cpp"
#include "common\Engine\Complex Mesh\Vertex.cpp"
#include "common\Engine\Complex Mesh\ComplexMesh.cpp"

#include "common\Engine\D3D9 Objects\D3D9RenderTargetTexture.cpp"
#include "common\Engine\D3D9 Objects\D3D9RenderTargetSurface.cpp"
#include "common\Engine\D3D9 Objects\D3D9VertexDeclaration.cpp"
#include "common\Engine\D3D9 Objects\D3D9PixelShader.cpp"
#include "common\Engine\D3D9 Objects\D3D9VertexShader.cpp"
#include "common\Engine\D3D9 Objects\D3D9PersistentMesh.cpp"
#include "common\Engine\D3D9 Objects\D3D9Font.cpp"
#include "common\Engine\D3D9 Objects\D3D9Texture.cpp"
#include "common\Engine\D3D9 Objects\D3D9Primitives.cpp"

#include "common\Engine\Graphics Devices\GraphicsDevice.cpp"
#include "common\Engine\Graphics Devices\D3D9GraphicsDevice.cpp"

#include "common\Engine\Utility\MovieExporter.cpp"
#include "common\Engine\Utility\AudioCapture.cpp"
#include "common\Engine\Utility\VideoCompressor.cpp"
#include "common\Engine\Utility\FileCollection.cpp"
#include "common\Engine\Utility\ParameterFile.cpp"
#include "common\Engine\Utility\FrameTimer.cpp"
#include "common\Engine\Utility\Profiler.cpp"
#include "common\Engine\Utility\Directory.cpp"
//#include "common\Engine\Utility\MenuInterface.cpp"
#include "common\Engine\Utility\Console.cpp"

//#include "common\Engine\Windows Controller\ApplicationWindow.cpp"
//#include "common\Engine\Windows Controller\InputManager.cpp"
//#include "common\Engine\Windows Controller\EventHandler.cpp"
//#include "common\Engine\Windows Controller\WinMain.cpp"
