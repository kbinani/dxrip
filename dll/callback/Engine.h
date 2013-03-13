/*
Engine.h
Written by Matthew Fisher

Many objects are universal and don't need to change often.  The source for all such files
are included in this file.  Often this would be compiled into a single *.lib or *.dll,
but I prefer to keep all my source readily accessible.  Another advantage of this is that
it is simpler to compile a subset of the code that includes only desired functionality (and
thus avoids as many external dependencies as possible.)
*/

/****************************/
/*           Core           */
/****************************/
//These are generic objects and functions

//Forward declarations for several classes/structures
#include "common\Engine\Core\ClassList.h"

//All #includes that are generic or written by other people
#include "common\Engine\Core\ExternalFiles.h"

//The Vector class contains dynamic arrays of a template type.  This is equivalent to the Standard Template 
//Library's vector class.
#include "common\Engine\Core\Vector.h"

//Collection of useful constants, macros, and functions
#include "common\Engine\Core\Stdhdr.h"

//Generic 32-bit RGBA color structure.  This early include is needed by SpaceVector
#include "common\Engine\Math\RGBColor.h"

//SpaceVector, which defines 2D, 3D and 4D vectors and rectangles.  This early include is needed by Grid
#include "common\Engine\Math\SpaceVector.h"

//a Grid is a 2D array
#include "common\Engine\Core\Grid.h"

//Basic string class
#include "common\Engine\Core\String.h"
#include "common\Engine\Core\UnicodeString.h"

//OutputDataStream and InputDataStream are binary equivalents of ofstream and ifstream, respectively
#include "common\Engine\Core\OutputDataStream.h"
#include "common\Engine\Core\InputDataStream.h"

#include "common\Engine\Core\Compression.h"

//definition of various key codes
#include "common\Engine\Core\KeyDefs.h"

/****************************/
/*          Audio           */
/****************************/
//Objects pretaining to loading and playing audio files

//WaveFileReader is needed to load wave files from disk
#include "common\Engine\Audio\WaveFileReader.h"

//a wave file defines a (possibly stereo) sound
#include "common\Engine\Audio\WaveFile.h"

//AudioDevice plays wave files
#include "common\Engine\Audio\AudioDevice.h"

/****************************/
/*      Multithreading      */
/****************************/
//Objects pretaining to creating applications that use multiple threads

//A mutex is an object that grants a thread exclusive access to a given section of code
#include "common\Engine\Multithreading\Mutex.h"

//A thread is a single unit of code execution
#include "common\Engine\Multithreading\Thread.h"

/****************************/
/*        Networking        */
/****************************/
//Objects pretaining to sending data between programs or computers

//A pipe is a connection between two programs, possibly on different computers
#include "common\Engine\Networking\Pipe.h"

/****************************/
/*           Math           */
/****************************/
//All mathematical structures and functions are here

//standard geometric objects
#include "common\Engine\Math\Line3D.h"
#include "common\Engine\Math\LineSegment2D.h"
#include "common\Engine\Math\LineSegment3D.h"
#include "common\Engine\Math\Ray3D.h"
#include "common\Engine\Math\Plane.h"
#include "common\Engine\Math\Intersect.h"
#include "common\Engine\Math\Distance.h"

//fixed size square matrix classes
#include "common\Engine\Math\Matrix4.h"

//Dense matrix class
#include "common\Engine\Math\DenseMatrix.h"

//Sparse matrix class, used by LinearSolver
#include "common\Engine\Math\SparseMatrix.h"

//Linear optimizer.  Uses (bi-)conjugate gradient descent or TAUCS's cholesky solver
#include "common\Engine\Math\LinearSolver.h"

#include "common\Engine\Math\PCA.h"

#include "common\Engine\Math\KMeansClustering.h"

/****************************/
/*     Graphics Objects     */
/****************************/
//These are structures specific to geometry and computer graphics rendering

//A bitmap class (a 2D array of RGBColor's)
#include "common\Engine\Graphics Objects\Bitmap.h"

//MatrixController manages the 3 transformations (world, view, projection) central to the graphics pipeline
#include "common\Engine\Graphics Objects\MatrixController.h"

//Camera represents a 3-D free floating camera.
//#include "common\Engine\Graphics Objects\Camera.h"

//MeshVertex represents a single vertex in a mesh structure
#include "common\Engine\Graphics Objects\MeshVertex.h"

//General purpose nearest-neighbor structure
#include "common\Engine\Graphics Objects\KDTreeN.h"

//3D point nearest neighbor structure
#include "common\Engine\Graphics Objects\KDTree3.h"

//Ray and triangle mesh intersection structure
#include "common\Engine\Graphics Objects\RayIntersector.h"
#include "common\Engine\Graphics Objects\RayIntersectorBruteForce.h"
#include "common\Engine\Graphics Objects\RayIntersectorKDTree.h"

//collection of 3D points
#include "common\Engine\Graphics Objects\PointSet.h"

//Rendering functions for lines and polygons in software
#include "common\Engine\Graphics Objects\PrimitiveRender.h"

/****************************/
/*     Machine Learning     */
/****************************/
#include "common\Engine\MachineLearning\BinaryClassifier.h"
#include "common\Engine\MachineLearning\BinaryClassifierBagged.h"
#include "common\Engine\MachineLearning\BinaryClassifierAdaBoost.h"
#include "common\Engine\MachineLearning\BinaryClassifierLogisticRegression.h"
#include "common\Engine\MachineLearning\BinaryClassifierSVM.h"
#include "common\Engine\MachineLearning\BinaryClassifierNaiveBayes.h"
#include "common\Engine\MachineLearning\BinaryClassifierMulticlass.h"
#include "common\Engine\MachineLearning\MulticlassClassifier.h"
#include "common\Engine\MachineLearning\MulticlassClassifierAdaBoostM1.h"
#include "common\Engine\MachineLearning\MulticlassClassifierPairwiseCoupling.h"
#include "common\Engine\MachineLearning\MulticlassClassifierOneVsAll.h"
#include "common\Engine\MachineLearning\MulticlassClassifierDecisionTree.h"
#include "common\Engine\MachineLearning\MulticlassClassifierNearestNeighborBruteForce.h"
#include "common\Engine\MachineLearning\MulticlassClassifierNearestNeighborANN.h"
#include "common\Engine\MachineLearning\MulticlassClassifierGenerator.h"
#include "common\Engine\MachineLearning\RegressionLearner.h"
#include "common\Engine\MachineLearning\RegressionLearnerBagged.h"
#include "common\Engine\MachineLearning\RegressionLearnerDecisionTree.h"
#include "common\Engine\MachineLearning\RegressionLearnerNearestNeighbor.h"
#include "common\Engine\MachineLearning\ClassifierDatasetTransformer.h"
#include "common\Engine\MachineLearning\ClassifierDatasetTransformerPCA.h"

/****************************/
/*  Simple Mesh Structures  */
/****************************/
//These files all center around creating and manipulating triangle meshes.
//These meshes are "simple" in that they contain no adjaceny information and are just an indexed vertex list.

//BaseMesh is an abstract mesh class that defines basic mesh functionality.  It also includes source for most
//of the manipulation (shape generation, file loading, etc.) that is possible under this generic structure.
#include "common\Engine\Simple Mesh\BaseMesh.h"

//D3D9Mesh is a DirectX 9 implementation of the BaseMesh class
#include "common\Engine\Simple Mesh\D3D9Mesh.h"

//Mesh is a standard C++ array instance of the BaseMesh class.  It's the only kind that can be used by OpenGL,
//but it will run fine under DirectX as well (if not as quickly as a D3D9Mesh.)
#include "common\Engine\Simple Mesh\Mesh.h"

/************************************/
/*     Complex Mesh Structures      */
/************************************/
//These files implement a complex (edge-based) mesh structure.

//definition of the Triangle structure
#include "common\Engine\Complex Mesh\Triangle.h"

//functions for an indivudial vertex
#include "common\Engine\Complex Mesh\Vertex.h"

//Edge-based functions
#include "common\Engine\Complex Mesh\FullEdge.h"

//ComplexMesh class encapsulates and stores arrays of all components in the edge-based data structure
#include "common\Engine\Complex Mesh\ComplexMesh.h"

/****************************/
/*     Direct3D Objects     */
/****************************/
//Objects that encapsulate shader and render-to-texture functionality in DirectX.

//Abstract class used for all resetable D3D9 graphics objects
#include "common\Engine\D3D9 Objects\GraphicsAsset.h"

//A texture that can be used as the render target
#include "common\Engine\D3D9 Objects\D3D9RenderTargetTexture.h"

//A surface that can be used as the render target
#include "common\Engine\D3D9 Objects\D3D9RenderTargetSurface.h"

//Vertex decleration used for shaders
#include "common\Engine\D3D9 Objects\D3D9VertexDeclaration.h"

//Pixel shaders
#include "common\Engine\D3D9 Objects\D3D9PixelShader.h"

//Vertex shaders
#include "common\Engine\D3D9 Objects\D3D9VertexShader.h"

//Fonts
#include "common\Engine\D3D9 Objects\D3D9Font.h"

//D3D9Mesh that can reset itself with a RAM copy
#include "common\Engine\D3D9 Objects\D3D9PersistentMesh.h"

#include "common\Engine\D3D9 Objects\D3D9Texture.h"

#include "common\Engine\D3D9 Objects\D3D9Primitives.h"

/****************************/
/*     Graphics Devices     */
/****************************/
//A graphics device controls the interaction between Windows and a graphics API.

//GraphicsDevice is the abstract class that contains functionality that all graphics devices must expose.
//any GraphicsDevice can do simple things like render meshes into the main rendering window and handle
//the world, view and projection transforms.
#include "common\Engine\Graphics Devices\GraphicsDevice.h"

//a D3D9 instance of GraphicsDevice
#include "common\Engine\Graphics Devices\D3D9GraphicsDevice.h"

/****************************/
/*          Utility         */
/****************************/
//These simplify access to existing classes

//The frame timer reads the system time and maintains the frames per second counter.
#include "common\Engine\Utility\FrameTimer.h"

//MovieExporter manages screen capture and exports as a sequence of images for compilation
//into a movie.
#include "common\Engine\Utility\MovieExporter.h"

//AudioCapture captures the current line-out or line-in to an uncompressed audio stream
#include "common\Engine\Utility\AudioCapture.h"

//VideoCompressor takes a sequence of images and compressed them into a video file.
#include "common\Engine\Utility\VideoCompressor.h"

//A FileCollection stores a large number of files as a single file.  It is similar to a tar file in functionality.
#include "common\Engine\Utility\FileCollection.h"

//A directory manages the list of files and subdirectories in a given folder
#include "common\Engine\Utility\Directory.h"

//the Parameter class loads a paramater file as a simple set of "Parameter=Option" lines
#include "common\Engine\Utility\ParameterFile.h"

//Profiler is used to record the percent of time spent in code
#include "common\Engine\Utility\Profiler.h"

//MenuInterface streamlines adding a menu to a window
//#include "common\Engine\Utility\MenuInterface.h"

//The Console namespace controls all access to the console
#include "common\Engine\Utility\Console.h"

/****************************/
/*    Windows Controllers   */
/****************************/
//These files handle the interaction between the application and Windows.  This includes files that contain
//WinMain and create the main application window.  There is also a frame timer and a screen capture mechanism.

//The input manager stores and controls access to keyboard and mouse data
//#include "common\Engine\Windows Controller\InputManager.h"

//Creating a new window involves having a callback function, WndProc, that is called whenever Windows has
//an event to send to your window/application.  EventHandler contains the WndProc file as well as the global
//array of keys that tells which keys are current pressed/depressed.
//#include "common\Engine\Windows Controller\EventHandler.h"

//Window handles creating and managing the application's main window
//#include "common\Engine\Windows Controller\ApplicationWindow.h"
