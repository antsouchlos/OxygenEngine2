//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
#include "FEngine.h"
#include <iostream>
#include <typeinfo>
#include <fstream>

/*
 * this is a sample application to demonstrate usage of the FluorinEngine API
 *
 */

/*
 *  -----------------------------------ATTENTION - IMPORTANT-----------------------------------
 * | I will be using this file to test the Carbon_Writer and Carbon_Parser, so don't wonder if |
 * | sometimes the application does not do what you intended, because it won't.                |
 * |                                                                                           |
 * | PS.                                                                                       |
 * | I am deleting this MOTHERFUCKING "exited cleanly", because it prints that every fucking   |
 * | time, even if the program was forced to shutdown with "CTRL-C", wich is, by definition,   |
 * | NOT A CLEAN EXIT.                                                                         |
 * |                                                                                           |
 * | I am also going to comment out the "int main()" at the bottom of the file and write a new |
 * | one at the top, because I don't want to scroll down every time I open this file to make   |
 * | a change.                                                                                 |
 *  -------------------------------------------------------------------------------------------
 */

/*  --------------------------------ANSWER TO THE ABOVE------------------------------------
 * |1. Well i will bring back the "Exited cleanly because you do not understand its purpose|
 * |   For me it doesnt say "exited cleanly" when CTRL-Cing it.                            |
 *  ---------------------------------------------------------------------------------------
 */

/*  ----------------------------------------FURTHER UPDATES------------------------------------------
 * | 1. At least make it say "Done" instead of "exited cleanly", because the exit might not be clean |
 * | 2. I uncommented the "int main()" at the bottom of the file an erased the one at the top, since |
 * | I am not going to be using "main.cpp" for testing very soon                                     |
 *  -------------------------------------------------------------------------------------------------
*/

/* |-------------------------------------------------------------------------------------------------|
 * |-----------------------------------------VERY IMPORTANT------------------------------------------|
 * |-------------------------------------------------------------------------------------------------|
 * |---------------------------------------EVEN MORE UPDATES-----------------------------------------|
 * | I finally finished the CSL_Interpreter, so now its time for testing.                            |
 * |                                                                                                 |
 * | (That means that most of tht time "main.cpp" will propably not do what you intended,            |
 * | since I'll be using it for testing and possibly debugging)                                      |
 * |                                                                                                 |
 * | AND THAT ALSO MEANS THAT YOU SHOULD GET THE CROSS-COMPILATION DONE, OR SOMEHOW MAKE IT SO THAT  |
 * | I CAN COMPILE IT, SO I CAN ACTUALLY TEST THE INTERPRETER                                        |
 * |-------------------------------------------------------------------------------------------------|
 * |-------------------------------------------------------------------------------------------------|
 * |-------------------------THANK YOU FOR READING THIS VERY IMPORTANT NOTE--------------------------|
 * |-------------------------------------------------------------------------------------------------|
 */

/* -------------------------------------------PS----------------------------------------------
 * I think I have found a way to make the project compile I am changing the "mode" variable
 * in "configure"  from "win64" to "g++"
 * And as far as I can see it does actually compile correctly, so that is what I'll be doing
 * to compile the project
 * -------------------------------------------------------------------------------------------
 */
 
 
 /****************************************************************************************************/
/*--------------------------------------------PS2---------------------------------------------
 * i have set the configure to 'win64' again for testing.. almost ready, 
 * compiles but does not run on wine (due to glew32.dll not initializing correctly) writing this on 2:22 AM
 * will look into the issue (and add sys.argv)
 *--------------------------------------------------------------------------------------------
 */
 
/*****************************************************************************************************/
/*--------------------------------------------PS3---------------------------------------------
 * Added command-line options to ./configure namely --win64 and --linux64 (linux64 default)
 * Work-in-progress to enable changing object file directory
 *--------------------------------------------------------------------------------------------
 */

//'STOI' ERROR FIXED: TCM.MODES SHOULD BE AN INTEGER VECTOR, NOT A STRING VECTOR.
//BTW THE ERROR WAS NOT IN THE INTERPRETER BUT IN THE SOURCE-CODE


class Game;

int updateA(void*, FTask);
int loadFileA(void*, FTask);

class Game: public FEngine{
  public:
    int variable;
    FE_Mat4 model_matrix;
    FE_Mat4 view_matrix = FE_Math::view(FE_Vec4(0, 0, 2, 1), FE_Vec4(0, 0, 0, 0));
    FE_Mat4 perspective_matrix = FE_Math::perspective(67, 4.0/3.0, 0.1, 100);
    FE_Quat quaternion = FE_Quat(FE_Vec4(0.0f, 0.0f, 1.0f, 0.0f));
    FE_Vec4 cam_pos = FE_Vec4(0, 0, 35, 1);

    float i = 0.0f;
    int o=0;

    Game(){


        // this is needed to initialize the engine
        this->Init("Fluorine Engine : Demo", 800, 600, false, FE_OPENGL3_2);

        this->variable = 0;

        //this->AddControllers();


        /*const GLchar* vertex_shader_data = Shader(

        in vec3 vertex;
        in vec3 normals;

        uniform mat4 perspective;
        uniform mat4 view;
        uniform mat4 model;


        //out vec3 material_params;
        out vec4 pos_eye;
        out vec4 normal_eye;

        void main () {
             //color = vec3(1.0, 0.0, 0.0);


             pos_eye     = view * model * vec4(vertex, 1.0f);
             normal_eye  = view * model * vec4(normals, 0.0f);
             //mat4 pa = mat4(vec4(1.133f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 1.51f, 0.0f, 0.0f), vec4(0.0f, 0.0f, -1.002f, -0.2f), vec4(0.0f, 0.0f, -1.0f, 0.0f));
             gl_Position = perspective * view  * model * vec4(vertex, 1.0f);
             //gl_Position = FE_Vec4(0.0, 0.0, 0.0, 1.0);
        }
        );
        // fragment shader
        const GLchar* fragment_shader_data = Shader(

        in vec4 pos_eye;
        in vec4 normal_eye;
        uniform samplerBuffer material;

        out vec4 frag_color;

        vec3 shading(in vec3 light, in vec3 pos, in vec3 normal,in vec3 diffuse_intensity,in int specular_intensity){

            vec3 n = normalize(normal);
            vec3 s = normalize(light - pos);
            vec3 v = normalize(-pos);

            //vec3 r = reflect(-s, tnorm);
            float sDotN = max( dot(s,n), 0.0f );

            vec3 diffuse =  diffuse_intensity*sDotN;
            vec3 specular = vec3(0.0f);

            if(sDotN>0.0f){
                float outputa = pow(max(dot(normalize(v+s), n), 0.0f), specular_intensity);
                if(outputa < 0.8f) specular = vec3(outputa*0.2f);
                else specular = vec3(outputa);

            }
            return diffuse+specular;
        }

        vec3 toVec3(vec4 inputa){
            return inputa.xyz;
        }

        void main(){

            vec4 light_pos   = vec4(0.0f, 0.0f, 2.0f, 1.0f);
            vec3 material_prop = vec3(texelFetch(material, 0).r, texelFetch(material, 1).r, texelFetch(material, 2).r);
            vec3 material_params  = shading(toVec3(light_pos), toVec3(pos_eye), toVec3(normalize(normal_eye)), material_prop, 200);
            //vec3 final_color  = vec3(0.0, 1.0, 0.0);
             frag_color  = vec4(material_params, 1.0);
         }
        );*/

        vector<GLfloat> vertices = {0.0f,  0.5f,  0.0f,
                          0.5f, -0.5f,  0.0f,
                          -0.5f, -0.5f,  0.0f,//};
                            0.0f,  0.5f,  0.0f,
                          -0.5f, -0.5f,  0.0f,
                          -1.0f, 0.5f, -0.5f};
        //cout << "size" << sizeof(vertices) << endl;
        vector<GLfloat> colors = {0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,//};

                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        -1.0f, 0.0f, 1.0f};

        vector<GLfloat> values = {0.7f, 0.7f, 0.7f, 0.0f};

        array<string, 4> vbosa = {"vertex", "normals", "\n", "\n"};

        //cout << (FEngine*)this << endl;
        //array<float, 16> view = view_matrix.toArray();
        /*for(unsigned int i = 0; i < 16; i++){
            if((i+1)%4 == 0 && i != 0)
                cout << view[i] << " "<<endl;
            else cout << view[i] << " ";
        }*/
        //cout << perspective_matrix.print(false) << endl;;
        /*
        FGLShaderManager* mng = this->GetShaderManager();
        mng->makeVBO("vertex", "vertex", vertices, 3, GL_STATIC_DRAW);
        mng->makeVBO("normals", "normals", colors, 3, GL_STATIC_DRAW);
        mng->makeVAO("vertexata", vbosa, 6);
        mng->makeTBO("texture", values);

        mng->makeSimpleProgram("main", vertex_shader_data, fragment_shader_data, "vertexata");
        mng->setUniform("main", "model", model_matrix.transpose());
        mng->setUniform("main", "perspective", perspective_matrix);
        mng->setUniform("main", "view", view_matrix.transpose());
        mng->setUniformTBO("main", "material", "texture");
        mng->setActive("main");
        */
        //GLint o = 0;
        //glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &o);
        //cout << o << endl;
		//pos={ 8.4811315536; -6.507639884; 5.3436651229;0.0 }
		//rot={ 1.1093189716; 0.0108169913; 0.8149281740;0.0 }
        //cout << glGetString(GL_VERSION) << endl;
        this->CreateNewThread("NEWTHREAD", false);
        this->AddTask("whatevera",  &updateA/*bind(&Game::update, this)*/ , this);
        this->AddTask("a",  &loadFileA/*bind(&Game::loadFile, this)*/, "NEWTHREAD" , this);
    }
    int loadFile(FTask task){
        this->loadWorld("fe_export_testaro_copy2.fle");
        this->setActive("omgftw");

        auto camera = this->getWorld()->getScene("omgftw")->getCamera("Camera");
        camera->setFOV(45.0);
        camera->setAspectRatio(4.0/3.0);
        camera->setRange(1, 500);
        camera->setZ(-8.0);
        camera->setY(-1.3);
        float adda = 0.0;
        for (auto obj : this->getWorld()->getScene("omgftw")->getObjectNames()) {
            //this->getWorld()->getScene("omgftw")->getObject(obj)->setZ(adda);
            adda++;
        }
        //this->deleteWorld();
        return 1;
    }
    // this is the function that runs every frame
    int update(FTask task){
        //cout << "RUNS WELL" << endl;
        //cout << o << endl;
        o++;
        //cout << "thread NR 000000000000" << endl;
        //FTask task = this->GetTaskInfo("default", "whatevera");
        //srand(SDL_GetTicks());
        //SDL_Delay(rand()%20);
        //cout << task.GetCounter() << endl;
        FKeyboard* kb = this->GetKeyboard();

        /*if(kb->IsPressed("s")){
            auto camera2 = this->getWorld()->getScene("omgftw")->getObject("Cube");
            camera2->setZ(camera2->getPos().z+0.1f);
        }

        if(kb->IsPressed("d")){
            auto camera2 = this->getWorld()->getScene("omgftw")->getObject("Cube");
            camera2->setX(camera2->getPos().x+0.1f);
        }

        if(kb->IsPressed("a")){
            auto camera2 = this->getWorld()->getScene("omgftw")->getObject("Cube");
            camera2->setX(camera2->getPos().x-0.1f);
        }
        if(kb->IsPressed("left")){
            auto camera2 = this->getWorld()->getScene("omgftw")->getObject("Cube.001");
            camera2->setX(camera2->getPos().x-0.1f);

        }
        if(kb->IsPressed("right")){
            auto camera2 = this->getWorld()->getScene("omgftw")->getObject("Cube.001");
            camera2->setX(camera2->getPos().x+0.1f);
        }
        if(kb->IsJustPressed("space")){
                cout << "MAKING RENDERER" << endl;
                SDL_Delay(1);
            restartRenderer(FE_OPENGL3_2);
        }
        if(kb->IsPressed("w")){
            //cout << "la ahogada" << endl;
            //restartRenderer(FE_OPENGL3_2);
            //cout << task->GetElapsedTime() << " " << task->GetTime() << endl;
            i+= 1;

            //auto camera = this->getWorld()->getScene("omgftw")->getObject("Cube.001");
            auto camera2 = this->getWorld()->getScene("omgftw")->getObject("Cube");
            /*for(auto obj: this->getWorld()->getScene("omgftw")->getObjectNames()){
                auto objecta = this->getWorld()->getScene("omgftw")->getObject(obj);
                objecta->setPos(FE_Vec4(0.0f, 0.0f, 0.0f, 0.0f));
                objecta->setRotEuler(FE_Vec4(0.0f, 0.0f, 0.0f, 0.0f));
            }*/
            //camera2->setZ(camera2->getPos().z-0.1f);
            //camera2->setZ(-10);
            //camera2->setX(1);
            //cout << camera->getPos().z << endl;
            //this->model_matrix.setRot(FE_Vec4(0.0f, i, 0.0f, 1.0f));
            //quaternion = FE_Quat(FE_Vec4(i, 0.0f, 1.0f, 0.0f));
            //this->model_matrix.setRot(quaternion);
            //this->model_matrix.setPos(FE_Vec4(i/500.0f, 0.0f, 0.0f, 1.0f));
            //cout << std::cos(FE_Math::radians(i)) << endl;
            //this->GetShaderManager()->setUniform("main", "model", model_matrix.transpose());
            //vector<GLfloat> values = {0.0f, 0.0f, 0.0f};
            //for(unsigned int a=0; a < values.size(); a++){
            //    values[a] = (float)i/100.0;
            //}
            //cout << values[0] << " " << values[1] << " " << values [2] << endl;
            //this->GetShaderManager()->setTBO("texture", 0, values);
            //values.clear();
        //}
        //cout << joystick->getAxis(1) << " " << variable << endl;
        variable++;
        //cout << "running" << endl;
        //cout << this->variable << endl;;
        /*if(this->mouse->justPressed('l') ){
            this->mouse->show();
            cout << "left mouse button shows cursor" << endl;
        }

        if(this->mouse->justPressed('m'))
            cout << "middle mouse button shows mouse position: " << this->mouse->getX() << " " << this->mouse->getY() << endl;


        if(this->mouse->justPressed('r') ){
            this->mouse->hide();
            cout << "right mouse button hides cursor" << endl;
        }

        if( this->mouse->getWheelStatus() == 1)
            cout << "mouse wheel moved up" << endl;

        if( this->mouse->getWheelStatus() == -1)
            cout << "mouse wheel moved down" << endl;
        */

        return task.CONTINUE();
    }

};

int updateA(void* data, FTask task){
    Game* engine = static_cast<Game*>(data);
    //cout << "DOING IT" << endl;
    return engine->update(task);
}

int loadFileA(void* data, FTask task){
    Game* engine = static_cast<Game*>(data);
    //cout << "DOING IT" << endl;
    return engine->loadFile(task);
}

int main ( int argc, char** argv )
{

    Game myapp = Game();
    myapp.Start();

	/*CSL_Interpreter interpreter;
	CSL_Writer writer;

	//test the interpreter
	string input = "";
	string line;
	ifstream in("testInterpreter.fle");
	while (getline(in, line))
		input += line;
	in.close();

	FE_World *world = interpreter.interpret(input);

	//test the writer
	ofstream out("testWriter.fle");
	out << writer.output(world);
	out.close();*/

    return 0;
}
#ifdef FE_WINDOWS
#endif
