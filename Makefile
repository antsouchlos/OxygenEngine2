

all: CSL_Test

CXX = g++

CXXFLAGS = -Wall -g -Og -std=c++17 -I/usr/include -Iinclude

LDFLAGS = -lSDL2 -L/usr/lib -L/usr/local/lib

OBJ_FILES = obj/OE_Camera.o obj/OE_Light.o obj/OE_Material.o obj/OE_Mesh.o obj/OE_Object.o obj/OE_PolygonStorage.o obj/OE_Scene.o obj/OE_TCM.o obj/OE_Texture.o obj/OE_VertexGroup.o obj/OE_VertexStorage.o obj/OE_ViewportConfig.o obj/OE_World.o obj/OE_TypesBase.o obj/CSL_Lexer.o obj/CSL_Parser.o obj/CSL_interpreter.o obj/csl_main_test.o obj/OE_EventParser.o obj/OE_Event.o obj/OE_EventHandler.o obj/OE_InputEventHandler.o obj/OE_Task.o obj/OE_DummyClasses.o obj/OE_TaskManager.o obj/OE_MutexCondition.o obj/OE_Math.o obj/OE_API.o obj/NRE_RenderData.o obj/NRE_RendererMain.o obj/NRE_GPU_API.o obj/glad.o obj/NRE_GL3_API.o obj/OE_SDL_WindowSystem.o

obj/OE_SDL_WindowSystem.o: src/OE_SDL_WindowSystem.cpp
	$(CXX) -c $(CXXFLAGS) src/OE_SDL_WindowSystem.cpp -o obj/OE_SDL_WindowSystem.o

obj/glad.o: src/Renderer/glad.c
	$(CXX) -c $(CXXFLAGS) src/Renderer/glad.c -o obj/glad.o

obj/NRE_GL3_API.o: src/Renderer/NRE_GL3_API.cpp
	$(CXX) -c $(CXXFLAGS) src/Renderer/NRE_GL3_API.cpp -o obj/NRE_GL3_API.o
	
obj/NRE_GPU_API.o: src/Renderer/NRE_GPU_API.cpp
	$(CXX) -c $(CXXFLAGS) src/Renderer/NRE_GPU_API.cpp -o obj/NRE_GPU_API.o

obj/NRE_RendererMain.o: src/Renderer/NRE_RendererMain.cpp
	$(CXX) -c $(CXXFLAGS) src/Renderer/NRE_RendererMain.cpp -o obj/NRE_RendererMain.o

obj/NRE_RenderData.o: src/Renderer/NRE_RenderData.cpp
	$(CXX) -c $(CXXFLAGS) src/Renderer/NRE_RenderData.cpp -o obj/NRE_RenderData.o

obj/OE_MutexCondition.o: src/Events/OE_MutexCondition.cpp
	$(CXX) -c $(CXXFLAGS) src/Events/OE_MutexCondition.cpp -o obj/OE_MutexCondition.o

obj/OE_API.o: src/OE_API.cpp
	$(CXX) -c $(CXXFLAGS) src/OE_API.cpp -o obj/OE_API.o
	
obj/OE_Math.o: src/OE_Math.cpp
	$(CXX) -c $(CXXFLAGS) src/OE_Math.cpp -o obj/OE_Math.o
	
obj/OE_DummyClasses.o: src/OE_DummyClasses.cpp
	$(CXX) -c $(CXXFLAGS) src/OE_DummyClasses.cpp -o obj/OE_DummyClasses.o

obj/OE_TaskManager.o: src/OE_TaskManager.cpp
	$(CXX) -c $(CXXFLAGS) src/OE_TaskManager.cpp -o obj/OE_TaskManager.o

obj/OE_Task.o: src/OE_Task.cpp
	$(CXX) -c $(CXXFLAGS) src/OE_Task.cpp -o obj/OE_Task.o

obj/OE_EventHandler.o: src/Events/OE_EventHandler.cpp
	$(CXX) -c $(CXXFLAGS) src/Events/OE_EventHandler.cpp -o obj/OE_EventHandler.o

obj/OE_InputEventHandler.o: src/Events/OE_InputEventHandler.cpp
	$(CXX) -c $(CXXFLAGS) src/Events/OE_InputEventHandler.cpp -o obj/OE_InputEventHandler.o

obj/OE_EventParser.o: src/Events/OE_EventParser.cpp
	$(CXX) -c $(CXXFLAGS) src/Events/OE_EventParser.cpp -o obj/OE_EventParser.o

obj/OE_Event.o: src/Events/OE_Event.cpp
	$(CXX) -c $(CXXFLAGS) src/Events/OE_Event.cpp -o obj/OE_Event.o

obj/csl_main_test.o: csl_main_test.cpp
	$(CXX) -c $(CXXFLAGS) csl_main_test.cpp -o obj/csl_main_test.o

obj/CSL_interpreter.o: src/Carbon/CSL_Interpreter.cpp
	$(CXX) -c $(CXXFLAGS) src/Carbon/CSL_Interpreter.cpp -o obj/CSL_interpreter.o

obj/CSL_Parser.o: src/Carbon/CSL_Parser.cpp
	$(CXX) -c $(CXXFLAGS) src/Carbon/CSL_Parser.cpp -o obj/CSL_Parser.o

obj/CSL_Lexer.o: src/Carbon/CSL_Lexer.cpp
	$(CXX) -c $(CXXFLAGS) src/Carbon/CSL_Lexer.cpp -o obj/CSL_Lexer.o

obj/OE_TypesBase.o: src/types/OE_TypesBase.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_TypesBase.cpp -o obj/OE_TypesBase.o

obj/OE_Camera.o: src/types/OE_Camera.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Camera.cpp -o obj/OE_Camera.o
	
obj/OE_Light.o: src/types/OE_Light.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Light.cpp -o obj/OE_Light.o
	
obj/OE_Material.o: src/types/OE_Material.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Material.cpp -o obj/OE_Material.o
	
obj/OE_Mesh.o: src/types/OE_Mesh.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Mesh.cpp -o obj/OE_Mesh.o
	
obj/OE_Object.o: src/types/OE_Object.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Object.cpp -o obj/OE_Object.o
	
obj/OE_PolygonStorage.o: src/types/OE_PolygonStorage.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_PolygonStorage.cpp -o obj/OE_PolygonStorage.o
	
obj/OE_Scene.o: src/types/OE_Scene.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Scene.cpp -o obj/OE_Scene.o

obj/OE_TCM.o: src/types/OE_TCM.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_TCM.cpp -o obj/OE_TCM.o
	
obj/OE_Texture.o: src/types/OE_Texture.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_Texture.cpp -o obj/OE_Texture.o
	
obj/OE_VertexGroup.o: src/types/OE_VertexGroup.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_VertexGroup.cpp -o obj/OE_VertexGroup.o
	
obj/OE_ViewportConfig.o: src/types/OE_ViewportConfig.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_ViewportConfig.cpp -o obj/OE_ViewportConfig.o
	
obj/OE_VertexStorage.o: src/types/OE_VertexStorage.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_VertexStorage.cpp -o obj/OE_VertexStorage.o
	
obj/OE_World.o: src/types/OE_World.cpp
	$(CXX) -c $(CXXFLAGS) src/types/OE_World.cpp -o obj/OE_World.o

CSL_Test: $(OBJ_FILES)
	$(CXX) -o CSL_Test $(OBJ_FILES) $(LDFLAGS) 
	
clean:
	rm CSL_Test $(OBJ_FILES)
