#include <Carbon/CSL_Interpreter.h>

int main(){
        
    /*OE_World world;
    cout << world.to_str() << endl;
    
    OE_Scene scene;
    cout << scene.to_str() << endl;
    
    OE_Material m;
    cout << m.to_str() << endl;
    
    OE_TCM tcm;
    cout << tcm.to_str() << endl;
    
    OE_Texture tex;
    cout << tex.to_str() << endl;
    
    OE_Mesh32 mesh;
    cout << mesh.to_str() << endl;
    
    OE_Camera cam;
    cout << cam.to_str() << endl;
    
    OE_Light l;
    cout << l.to_str() << endl;*/
    CSL_Interpreter interpreter;
    auto t = clock();
    cout << "CSL TEST BEGIN" << endl;
    
    OE_World* world = interpreter.interpretFile("challenge_car.csl");
    t = clock();
    auto a = (world->to_str());
    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    delete world;
    
    //OE_WriteToLog(a);
    ofstream myfile;
    myfile.open ("challenge_car_copy.csl");
    myfile << a;
    myfile.close();
    
    t = clock();
    cout << "CSL TEST BEGIN 2" << endl;
    OE_World* world2 = interpreter.interpretFile("challenge_car_copy.csl");
    t = clock();
    auto b = (world2->to_str());
    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    delete world2;
    
    ofstream myfile2;
    myfile2.open ("challenge_car_copy2.csl");
    myfile2 << b;
    myfile2.close();
    
    t = clock();
    cout << "CSL TEST BEGIN 3" << endl;
    OE_World* world3 = interpreter.interpretFile("csl_very_large_object_test.csl");
    t = clock();
    auto c = (world3->to_str());
    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    //SDL_Delay(1000);
    delete world3;
    
    ofstream myfile3;
    myfile3.open ("csl_very_large_object_test_copy.csl");
    myfile3 << c;
    myfile3.close();
    
    return 0;
}
