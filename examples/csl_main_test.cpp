#include <OE/api_oe.h>
#include <OE/Carbon/interpreter.h>

using namespace std;
using namespace oe;


int main() {
//	std::string input = "";
//	std::string filename = "csl_very_large_object_test.csl";
//	std::ifstream f(filename);
//
//	std::string line;
//	while(getline(f, line))
//		input += line;
//
//	f.close();
//
//	auto e = csl::parse(input);


//	csl::Interpreter interpreter;

    try {

        std::string filename = "assets/csl_very_large_object_test.csl";

        oe::init();

        auto t = clock();

        csl::interpret_file(filename);

        cout << "[Performance] The whole thing:  " << (float) (clock() - t) / CLOCKS_PER_SEC << endl;

        oe::destroy();

    } catch(csl::unexpected_token_error_t& e) {
        std::cout << e.what() << std::endl;
    }
   

	return 0;

//    CSL_Interpreter* interpreter = new CSL_Interpreter();
//    auto t = clock();
//    cout << "CSL TEST BEGIN" << endl;
//    
//    
//    // These boilerplate lines of code are absolutely necessary for proper testing in
//    // the new format
//    std::shared_ptr<OE_World> world = interpreter->interpretFile("challenge_car.csl");
//    OE_World::objectsList.extend(interpreter->objectsList, true);
//    OE_World::materialsList.extend(interpreter->materialsList, true);
//    OE_World::texturesList.extend(interpreter->texturesList, true);
//    OE_World::tcmsList.extend(interpreter->tcmsList, true);
//    OE_World::viewportsList.extend(interpreter->viewportsList, true);
//    OE_World::scenesList.extend(interpreter->scenesList, true);
//    
//    t = clock();
//    auto a = (world->to_str());
//    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
//    world = nullptr;
//    
//    //OE_WriteToLog(a);
//    ofstream myfile;
//    myfile.open ("challenge_car_copy.csl");
//    myfile << a;
//    myfile.close();
//    
//    t = clock();
//    cout << "CSL TEST BEGIN 2" << endl;
//    delete interpreter;
//    interpreter = new CSL_Interpreter();
//    std::shared_ptr<OE_World> world2 = interpreter->interpretFile("challenge_car_copy.csl");
//    OE_World::objectsList.extend(interpreter->objectsList, true);
//    OE_World::materialsList.extend(interpreter->materialsList, true);
//    OE_World::texturesList.extend(interpreter->texturesList, true);
//    OE_World::tcmsList.extend(interpreter->tcmsList, true);
//    OE_World::viewportsList.extend(interpreter->viewportsList, true);
//    OE_World::scenesList.extend(interpreter->scenesList, true);
//    
//    t = clock();
//    auto b = (world2->to_str());
//    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
//    
//    world2 = nullptr;
//    
//    ofstream myfile2;
//    myfile2.open ("challenge_car_copy2.csl");
//    myfile2 << b;
//    myfile2.close();
//    
//    t = clock();
//    cout << "CSL TEST BEGIN 3" << endl;
//    delete interpreter;
//    interpreter = new CSL_Interpreter();
//    std::shared_ptr<OE_World> world3 = interpreter->interpretFile("csl_very_large_object_test.csl");
//    OE_World::objectsList.extend(interpreter->objectsList, true);
//    OE_World::materialsList.extend(interpreter->materialsList, true);
//    OE_World::texturesList.extend(interpreter->texturesList, true);
//    OE_World::tcmsList.extend(interpreter->tcmsList, true);
//    OE_World::viewportsList.extend(interpreter->viewportsList, true);
//    OE_World::scenesList.extend(interpreter->scenesList, true);
//    
//    t = clock();
//    auto c = (world3->to_str());
//    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
//    
//    world3 = nullptr;
//    delete interpreter;
//    /*t = clock();
//    cout << "CSL TEST BEGIN 4" << endl;
//    interpreter = new CSL_Interpreter();
//    world3 = interpreter->interpretFile("csl_2UVMAP_TEST.csl");
//    OE_World::objectsList.extend(interpreter->objectsList, true);
//    OE_World::materialsList.extend(interpreter->materialsList, true);
//    OE_World::texturesList.extend(interpreter->texturesList, true);
//    OE_World::tcmsList.extend(interpreter->tcmsList, true);
//    OE_World::viewportsList.extend(interpreter->viewportsList, true);
//    OE_World::scenesList.extend(interpreter->scenesList, true);
//    
//    t = clock();
//    auto d = (world3->to_str());
//    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
//    //SDL_Delay(1000);
//    
//    cout << "NRE VERTEX/INDEX BUFFERS " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
//    world3 = nullptr;//*/
//    
//    //ofstream myfile3;
//    //myfile3.open ("csl_very_large_object_test_copy.csl");
//    //myfile3 << c;
//    //myfile3.close();
//    
//    //SDL_Delay(6000);
//    
//	return 0;
}
