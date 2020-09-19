#linux64

all: FluorineEngine

windows:
	./remake.sh --win64

linux:
	./remake.sh --linux64

CC = ccache g++

CXXFLAGS = -Wall -g -O0 -std=c++11 -I/usr/include -Iinclude/Renderer -Iinclude/Events -Iinclude/Carbon -Iinclude/Hydrogen_Engine -Iinclude/types -Iinclude

OBJ_FILES = obj/linux64/main.o obj/linux64/FE_Event.o obj/linux64/FE_EventParser.o obj/linux64/FE_InputEventHandler.o obj/linux64/FE_EventHandler.o obj/linux64/FE_GPU_VertexLayout.o obj/linux64/FE_Renderer.o obj/linux64/FE_GPU_Thread.o obj/linux64/FE_GPU_Shader.o obj/linux64/FE_GPU_CommandQueue.o obj/linux64/FE_GPU_Program.o obj/linux64/FE_GPU_API.o obj/linux64/FE_MeshRenderData.o obj/linux64/FE_GPU_Uniform.o obj/linux64/FE_GL3_API.o obj/linux64/FE_GL3_VertexBuffer.o obj/linux64/FE_GL3_Thread.o obj/linux64/FE_GL3_CommandQueue.o obj/linux64/FE_GL3_Program.o obj/linux64/FE_GL3_VertexLayout.o obj/linux64/FE_GL3_Uniform.o obj/linux64/FE_GPU_Info.o obj/linux64/FE_GPU_VertexBuffer.o obj/linux64/FMouse.o obj/linux64/CSL_Interpreter.o obj/linux64/FKeyboard.o obj/linux64/FE_Vertex.o obj/linux64/FE_Texture.o obj/linux64/FE_Interaction.o obj/linux64/FE_Light.o obj/linux64/FE_Camera.o obj/linux64/FE_Material.o obj/linux64/FE_Polygon.o obj/linux64/FE_Animation.o obj/linux64/FE_Scene.o obj/linux64/FE_Mesh.o obj/linux64/FE_VertexGroup.o obj/linux64/FE_Object.o obj/linux64/FE_World.o obj/linux64/FE_TCM.o obj/linux64/FMath.o obj/linux64/FtaskManager.o obj/linux64/FError.o obj/linux64/FTask.o obj/linux64/Hydrogen_Engine.o obj/linux64/Collision_Engine.o obj/linux64/FWorldManager.o obj/linux64/FGamepad.o obj/linux64/FEngine.o

LDFLAGS = -lSDL2 -lGL -lGLEW -L/usr/lib -L/usr/local/lib

obj/linux64/main.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/FGamepad.h include/types/FE_Camera.h include/FTask.h include/Renderer/FE_GPU_Thread.h include/Renderer/FE_Renderer.h include/types/FE_Vertex.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h include/Carbon/CSL_Interpreter.h include/Renderer/FE_GPU_Uniform.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_API.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/types/FE_Animation.h include/FKeyboard.h include/FtaskManager.h include/types/FE_Light.h include/types/FE_TCM.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Events/FE_InputEventHandler.h include/FMouse.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/FEngine.h include/Carbon/CSL_Writer.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/types/FE_Scene.h include/FWorldManager.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h main.cpp include/FError.h include/Renderer/FE_MeshRenderData.h
	$(CC) -c $(CXXFLAGS) main.cpp -o obj/linux64/main.o

obj/linux64/FE_Event.o: src/Events/FE_Event.cpp include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/FError.h
	$(CC) -c $(CXXFLAGS) src/Events/FE_Event.cpp -o obj/linux64/FE_Event.o

obj/linux64/FE_EventParser.o: include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/FError.h src/Events/FE_EventParser.cpp
	$(CC) -c $(CXXFLAGS) src/Events/FE_EventParser.cpp -o obj/linux64/FE_EventParser.o

obj/linux64/FE_InputEventHandler.o: include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Events/FE_Event.h src/Events/FE_InputEventHandler.cpp include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/Events/FE_InputEventHandler.cpp -o obj/linux64/FE_InputEventHandler.o

obj/linux64/FE_EventHandler.o: include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Events/FE_Event.h src/Events/FE_EventHandler.cpp include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/Events/FE_EventHandler.cpp -o obj/linux64/FE_EventHandler.o

obj/linux64/FE_GPU_VertexLayout.o: include/Renderer/FE_GPU_API.h src/Renderer/FE_GPU_VertexLayout.cpp include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_VertexLayout.cpp -o obj/linux64/FE_GPU_VertexLayout.o

obj/linux64/FE_Renderer.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/types/FE_Camera.h include/Renderer/FE_GPU_Thread.h include/types/FE_Vertex.h include/Renderer/FE_Renderer.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h include/Carbon/CSL_Interpreter.h include/Renderer/FE_GPU_Uniform.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_API.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/types/FE_Animation.h include/types/FE_Light.h include/types/FE_TCM.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/Carbon/CSL_Writer.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/types/FE_Scene.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h src/Renderer/FE_Renderer.cpp include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h include/Renderer/FE_MeshRenderData.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_Renderer.cpp -o obj/linux64/FE_Renderer.o

obj/linux64/FE_GPU_Thread.o: src/Renderer/FE_GPU_Thread.cpp include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_Thread.cpp -o obj/linux64/FE_GPU_Thread.o

obj/linux64/FE_GPU_Shader.o: src/Renderer/FE_GPU_Shader.cpp include/FError.h include/Renderer/FE_GPU_Shader.h include/types/FE_Libs.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_Shader.cpp -o obj/linux64/FE_GPU_Shader.o

obj/linux64/FE_GPU_CommandQueue.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/FE_GPU_CommandQueue.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_CommandQueue.cpp -o obj/linux64/FE_GPU_CommandQueue.o

obj/linux64/FE_GPU_Program.o: src/Renderer/FE_GPU_Program.cpp include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_Program.cpp -o obj/linux64/FE_GPU_Program.o

obj/linux64/FE_GPU_API.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/FE_GPU_API.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_API.cpp -o obj/linux64/FE_GPU_API.o

obj/linux64/FE_MeshRenderData.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/types/FE_Camera.h include/Renderer/FE_GPU_Thread.h include/types/FE_Vertex.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h include/Renderer/FE_GPU_VertexBuffer.h include/Carbon/CSL_Interpreter.h include/Renderer/FE_GPU_Uniform.h include/Renderer/FE_GPU_API.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/types/FE_Animation.h include/types/FE_Light.h src/Renderer/FE_MeshRenderData.cpp include/types/FE_TCM.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/Carbon/CSL_Writer.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/types/FE_Scene.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h include/Renderer/FE_MeshRenderData.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_MeshRenderData.cpp -o obj/linux64/FE_MeshRenderData.o

obj/linux64/FE_GPU_Uniform.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/FE_GPU_Uniform.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_Uniform.cpp -o obj/linux64/FE_GPU_Uniform.o

obj/linux64/FE_GL3_API.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/OpenGL3/FE_GL3_API.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_API.cpp -o obj/linux64/FE_GL3_API.o

obj/linux64/FE_GL3_VertexBuffer.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/OpenGL3/FE_GL3_VertexBuffer.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_VertexBuffer.cpp -o obj/linux64/FE_GL3_VertexBuffer.o

obj/linux64/FE_GL3_Thread.o: include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h src/Renderer/OpenGL3/FE_GL3_Thread.cpp include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_Thread.cpp -o obj/linux64/FE_GL3_Thread.o

obj/linux64/FE_GL3_CommandQueue.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/OpenGL3/FE_GL3_CommandQueue.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_CommandQueue.cpp -o obj/linux64/FE_GL3_CommandQueue.o

obj/linux64/FE_GL3_Program.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h src/Renderer/OpenGL3/FE_GL3_Program.cpp include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_Program.cpp -o obj/linux64/FE_GL3_Program.o

obj/linux64/FE_GL3_VertexLayout.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h src/Renderer/OpenGL3/FE_GL3_VertexLayout.cpp include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_VertexLayout.cpp -o obj/linux64/FE_GL3_VertexLayout.o

obj/linux64/FE_GL3_Uniform.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/OpenGL3/FE_GL3_Uniform.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/OpenGL3/FE_GL3_Uniform.cpp -o obj/linux64/FE_GL3_Uniform.o

obj/linux64/FE_GPU_Info.o: include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h src/Renderer/FE_GPU_Info.cpp include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_Info.cpp -o obj/linux64/FE_GPU_Info.o

obj/linux64/FE_GPU_VertexBuffer.o: include/Renderer/FE_GPU_API.h include/types/FE_Libs.h include/Events/FE_EventHandler.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/Events/FE_InputEventHandler.h include/Events/FE_Event.h include/Renderer/FE_GPU_Info.h src/Renderer/FE_GPU_VertexBuffer.cpp include/Renderer/FE_GPU_VertexLayout.h include/Renderer/FE_GPU_Thread.h include/FError.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_Uniform.h
	$(CC) -c $(CXXFLAGS) src/Renderer/FE_GPU_VertexBuffer.cpp -o obj/linux64/FE_GPU_VertexBuffer.o

obj/linux64/FMouse.o: include/FMouse.h include/Events/FE_EventHandler.h include/types/FE_Libs.h include/Events/FE_EventParser.h src/FMouse.cpp include/Events/FE_Event.h include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/FMouse.cpp -o obj/linux64/FMouse.o

obj/linux64/CSL_Interpreter.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/types/FE_Camera.h include/types/FE_Vertex.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h src/Carbon/CSL_Interpreter.cpp include/Carbon/CSL_Interpreter.h include/Events/FE_EventParser.h include/types/FE_Animation.h include/types/FE_Light.h include/types/FE_TCM.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/Carbon/CSL_Writer.h include/types/FE_Scene.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h
	$(CC) -c $(CXXFLAGS) src/Carbon/CSL_Interpreter.cpp -o obj/linux64/CSL_Interpreter.o

obj/linux64/FKeyboard.o: include/Events/FE_EventHandler.h include/types/FE_Libs.h include/Events/FE_EventParser.h src/FKeyboard.cpp include/Events/FE_Event.h include/FKeyboard.h include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/FKeyboard.cpp -o obj/linux64/FKeyboard.o

obj/linux64/FE_Vertex.o: src/types/FE_Vertex.cpp include/Events/FE_EventHandler.h include/FMath.h include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/FError.h include/types/FE_Vertex.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Vertex.cpp -o obj/linux64/FE_Vertex.o

obj/linux64/FE_Texture.o: include/types/FE_Libs.h src/types/FE_Texture.cpp include/types/FE_Texture.h include/FError.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Texture.cpp -o obj/linux64/FE_Texture.o

obj/linux64/FE_Interaction.o: include/FError.h include/types/FE_Interaction.h include/types/FE_Libs.h src/types/FE_Interaction.cpp
	$(CC) -c $(CXXFLAGS) src/types/FE_Interaction.cpp -o obj/linux64/FE_Interaction.o

obj/linux64/FE_Light.o: include/Events/FE_EventHandler.h include/FMath.h include/Events/FE_EventParser.h include/types/FE_Libs.h include/Events/FE_Event.h include/types/FE_Light.h include/types/FE_Object.h include/FError.h include/Events/FE_InputEventHandler.h src/types/FE_Light.cpp
	$(CC) -c $(CXXFLAGS) src/types/FE_Light.cpp -o obj/linux64/FE_Light.o

obj/linux64/FE_Camera.o: include/Events/FE_EventHandler.h include/FMath.h include/Events/FE_EventParser.h include/types/FE_Libs.h include/Events/FE_Event.h src/types/FE_Camera.cpp include/types/FE_Object.h include/types/FE_Camera.h include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Camera.cpp -o obj/linux64/FE_Camera.o

obj/linux64/FE_Material.o: include/types/FE_Material.h include/Events/FE_EventHandler.h include/FMath.h include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/FError.h include/Events/FE_InputEventHandler.h src/types/FE_Material.cpp
	$(CC) -c $(CXXFLAGS) src/types/FE_Material.cpp -o obj/linux64/FE_Material.o

obj/linux64/FE_Polygon.o: include/Events/FE_EventHandler.h include/FMath.h include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h src/types/FE_Polygon.cpp include/types/FE_Polygon.h include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Polygon.cpp -o obj/linux64/FE_Polygon.o

obj/linux64/FE_Animation.o: include/Events/FE_EventHandler.h include/types/FE_Libs.h include/FMath.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/types/FE_Animation.h src/types/FE_Animation.cpp include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Animation.cpp -o obj/linux64/FE_Animation.o

obj/linux64/FE_Scene.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/types/FE_Camera.h include/types/FE_Vertex.h include/types/FE_Interaction.h include/Events/FE_EventParser.h include/types/FE_Animation.h include/types/FE_Light.h include/types/FE_TCM.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/types/FE_Scene.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h src/types/FE_Scene.cpp include/FError.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Scene.cpp -o obj/linux64/FE_Scene.o

obj/linux64/FE_Mesh.o: include/types/FE_Vertex.h include/Events/FE_EventHandler.h include/FMath.h include/Events/FE_EventParser.h include/types/FE_Libs.h include/Events/FE_Event.h include/types/FE_VertexGroup.h include/types/FE_Animation.h src/types/FE_Mesh.cpp include/types/FE_Polygon.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h include/types/FE_Interaction.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Mesh.cpp -o obj/linux64/FE_Mesh.o

obj/linux64/FE_VertexGroup.o: include/FError.h include/types/FE_Libs.h include/types/FE_VertexGroup.h src/types/FE_VertexGroup.cpp
	$(CC) -c $(CXXFLAGS) src/types/FE_VertexGroup.cpp -o obj/linux64/FE_VertexGroup.o

obj/linux64/FE_Object.o: include/Events/FE_EventHandler.h include/FMath.h include/Events/FE_EventParser.h include/types/FE_Libs.h include/Events/FE_Event.h src/types/FE_Object.cpp include/types/FE_Object.h include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_Object.cpp -o obj/linux64/FE_Object.o

obj/linux64/FE_World.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/types/FE_Camera.h include/types/FE_Vertex.h include/types/FE_Interaction.h include/Events/FE_EventParser.h include/types/FE_Animation.h include/types/FE_Light.h include/types/FE_TCM.h include/Events/FE_InputEventHandler.h src/types/FE_World.cpp include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/types/FE_World.h include/types/FE_Scene.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h
	$(CC) -c $(CXXFLAGS) src/types/FE_World.cpp -o obj/linux64/FE_World.o

obj/linux64/FE_TCM.o: include/Events/FE_EventHandler.h include/types/FE_Libs.h include/FMath.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/types/FE_TCM.h include/FError.h src/types/FE_TCM.cpp include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/types/FE_TCM.cpp -o obj/linux64/FE_TCM.o

obj/linux64/FMath.o: include/Events/FE_EventHandler.h include/FMath.h include/Events/FE_EventParser.h include/types/FE_Libs.h include/Events/FE_Event.h src/FMath.cpp include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/FMath.cpp -o obj/linux64/FMath.o

obj/linux64/FtaskManager.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/types/FE_Camera.h include/FTask.h include/Renderer/FE_GPU_Thread.h include/types/FE_Vertex.h include/Renderer/FE_Renderer.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h include/Carbon/CSL_Interpreter.h include/Renderer/FE_GPU_Uniform.h include/Renderer/FE_GPU_VertexBuffer.h src/FtaskManager.cpp include/Renderer/FE_GPU_API.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/types/FE_Animation.h include/FtaskManager.h include/types/FE_Light.h include/types/FE_TCM.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/Carbon/CSL_Writer.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/types/FE_Scene.h include/FWorldManager.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h include/Renderer/FE_MeshRenderData.h
	$(CC) -c $(CXXFLAGS) src/FtaskManager.cpp -o obj/linux64/FtaskManager.o

obj/linux64/FError.o: include/FError.h src/FError.cpp
	$(CC) -c $(CXXFLAGS) src/FError.cpp -o obj/linux64/FError.o

obj/linux64/FTask.o: include/Events/FE_EventHandler.h include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h src/FTask.cpp include/FTask.h include/FError.h include/Events/FE_InputEventHandler.h
	$(CC) -c $(CXXFLAGS) src/FTask.cpp -o obj/linux64/FTask.o

obj/linux64/Hydrogen_Engine.o: include/types/FE_Texture.h include/Hydrogen_Engine/HE_Types.h include/types/FE_Polygon.h include/types/FE_Camera.h src/Hydrogen_Engine/Hydrogen_Engine.cpp include/types/FE_Vertex.h include/types/FE_Interaction.h include/Events/FE_EventParser.h include/types/FE_Animation.h include/Hydrogen_Engine/Hydrogen_Engine.h include/types/FE_Light.h include/types/FE_TCM.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/types/FE_World.h include/types/FE_Scene.h include/types/FE_Material.h include/Hydrogen_Engine/Collision_Engine.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h
	$(CC) -c $(CXXFLAGS) src/Hydrogen_Engine/Hydrogen_Engine.cpp -o obj/linux64/Hydrogen_Engine.o

obj/linux64/Collision_Engine.o: include/types/FE_Texture.h include/Hydrogen_Engine/HE_Types.h include/types/FE_Polygon.h include/types/FE_Camera.h src/Hydrogen_Engine/Collision_Engine.cpp include/types/FE_Vertex.h include/types/FE_Interaction.h include/Events/FE_EventParser.h include/types/FE_Animation.h include/types/FE_Light.h include/types/FE_TCM.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/types/FE_World.h include/types/FE_Scene.h include/types/FE_Material.h include/Hydrogen_Engine/Collision_Engine.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h
	$(CC) -c $(CXXFLAGS) src/Hydrogen_Engine/Collision_Engine.cpp -o obj/linux64/Collision_Engine.o

obj/linux64/FWorldManager.o: include/types/FE_Texture.h include/types/FE_Polygon.h src/FWorldManager.cpp include/types/FE_Camera.h include/Renderer/FE_GPU_Thread.h include/types/FE_Vertex.h include/Renderer/FE_Renderer.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h include/Carbon/CSL_Interpreter.h include/Renderer/FE_GPU_Uniform.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_API.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/types/FE_Animation.h include/types/FE_Light.h include/types/FE_TCM.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Events/FE_InputEventHandler.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/Carbon/CSL_Writer.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/types/FE_Scene.h include/FWorldManager.h include/types/FE_Material.h include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h include/Renderer/FE_MeshRenderData.h
	$(CC) -c $(CXXFLAGS) src/FWorldManager.cpp -o obj/linux64/FWorldManager.o

obj/linux64/FGamepad.o: include/Events/FE_EventHandler.h include/types/FE_Libs.h include/Events/FE_EventParser.h include/Events/FE_Event.h include/FGamepad.h include/FError.h include/Events/FE_InputEventHandler.h src/FGamepad.cpp
	$(CC) -c $(CXXFLAGS) src/FGamepad.cpp -o obj/linux64/FGamepad.o

obj/linux64/FEngine.o: include/types/FE_Texture.h include/types/FE_Polygon.h include/FGamepad.h include/types/FE_Camera.h include/FTask.h include/Renderer/FE_GPU_Thread.h include/Renderer/FE_Renderer.h include/types/FE_Vertex.h include/types/FE_Interaction.h include/Carbon/CScanner_CLexer.h include/Carbon/CSL_Interpreter.h include/Renderer/FE_GPU_Uniform.h include/Renderer/FE_GPU_VertexBuffer.h include/Renderer/FE_GPU_API.h include/Events/FE_EventParser.h include/Renderer/FE_GPU_CommandQueue.h include/types/FE_Animation.h include/FKeyboard.h include/FtaskManager.h include/types/FE_Light.h include/types/FE_TCM.h include/Renderer/FE_GPU_Info.h include/Renderer/FE_GPU_VertexLayout.h include/Events/FE_InputEventHandler.h include/FMouse.h include/Events/FE_EventHandler.h include/types/FE_VertexGroup.h include/Events/FE_Event.h include/Carbon/Exceptions.h include/types/FE_World.h include/Carbon/CSL_Parser.h include/FEngine.h include/Carbon/CSL_Writer.h include/Renderer/FE_GPU_Program.h include/Renderer/FE_GPU_Shader.h include/types/FE_Scene.h include/FWorldManager.h include/types/FE_Material.h src/FEngine.cpp include/types/FE_Libs.h include/FMath.h include/types/FE_Object.h include/types/FE_Mesh.h include/FError.h include/Renderer/FE_MeshRenderData.h
	$(CC) -c $(CXXFLAGS) src/FEngine.cpp -o obj/linux64/FEngine.o

FluorineEngine: $(OBJ_FILES)
	$(CC) -o FluorineEngine $(OBJ_FILES) $(LDFLAGS) 

clean: 
	rm FluorineEngine obj/linux64/main.o obj/linux64/FE_Event.o obj/linux64/FE_EventParser.o obj/linux64/FE_InputEventHandler.o obj/linux64/FE_EventHandler.o obj/linux64/FE_GPU_VertexLayout.o obj/linux64/FE_Renderer.o obj/linux64/FE_GPU_Thread.o obj/linux64/FE_GPU_Shader.o obj/linux64/FE_GPU_CommandQueue.o obj/linux64/FE_GPU_Program.o obj/linux64/FE_GPU_API.o obj/linux64/FE_MeshRenderData.o obj/linux64/FE_GPU_Uniform.o obj/linux64/FE_GL3_API.o obj/linux64/FE_GL3_VertexBuffer.o obj/linux64/FE_GL3_Thread.o obj/linux64/FE_GL3_CommandQueue.o obj/linux64/FE_GL3_Program.o obj/linux64/FE_GL3_VertexLayout.o obj/linux64/FE_GL3_Uniform.o obj/linux64/FE_GPU_Info.o obj/linux64/FE_GPU_VertexBuffer.o obj/linux64/FMouse.o obj/linux64/CSL_Interpreter.o obj/linux64/FKeyboard.o obj/linux64/FE_Vertex.o obj/linux64/FE_Texture.o obj/linux64/FE_Interaction.o obj/linux64/FE_Light.o obj/linux64/FE_Camera.o obj/linux64/FE_Material.o obj/linux64/FE_Polygon.o obj/linux64/FE_Animation.o obj/linux64/FE_Scene.o obj/linux64/FE_Mesh.o obj/linux64/FE_VertexGroup.o obj/linux64/FE_Object.o obj/linux64/FE_World.o obj/linux64/FE_TCM.o obj/linux64/FMath.o obj/linux64/FtaskManager.o obj/linux64/FError.o obj/linux64/FTask.o obj/linux64/Hydrogen_Engine.o obj/linux64/Collision_Engine.o obj/linux64/FWorldManager.o obj/linux64/FGamepad.o obj/linux64/FEngine.o