#include <OE_API.h>

using namespace std;
using namespace OE;

int main(){
        
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
    t = clock();
    std::vector<float> vertex_buf;
    std::vector<uint32_t> index_buf;
    for (const auto& scene : world2->scenes){
        for (const auto& obj : scene.second->objects){
            if (obj.second->getType() == "MESH32"){
                OE_Mesh32* mesh = (OE_Mesh32*)obj.second;
                vertex_buf = mesh->data.genVertexBuffer();
                for(const auto& vgroup : mesh->data.triangle_groups){
                    index_buf = mesh->data.genIndexBuffer(vgroup.first);
                }
            }
        }
    }
    cout << "NRE VERTEX/INDEX BUFFERS " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
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
    
    t = clock();
    std::vector<float> vertex_buf2;
    std::vector<uint32_t> index_buf2;
    for (const auto& scene : world3->scenes){
        for (const auto& obj : scene.second->objects){
            if (obj.second->getType() == "MESH32"){
                OE_Mesh32* mesh = (OE_Mesh32*)obj.second;
                
                t = clock();
                vertex_buf2 = mesh->data.genVertexBuffer();
                cout << "NRE VERTEX BUFFERS " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
                t = clock();
                for(const auto& vgroup : mesh->data.triangle_groups){
                    index_buf2 = mesh->data.genIndexBuffer(vgroup.first);
                }
                cout << "NRE INDEX BUFFERS " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
            }
        }
    }
    
    delete world3;
    
    /*t = clock();
    cout << "CSL TEST BEGIN 4" << endl;
    world3 = interpreter.interpretFile("csl_2UVMAP_TEST.csl");
    t = clock();
    auto d = (world3->to_str());
    cout << "CSL TEST WRITER " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    //SDL_Delay(1000);
    
    t = clock();
    std::vector<float> vertex_buf3;
    std::vector<uint32_t> index_buf3;
    for (const auto& scene : world3->scenes){
        for (const auto& obj : scene.second->objects){
            if (obj.second->getType() == "MESH32"){
                OE_Mesh32* mesh = (OE_Mesh32*)obj.second;
                vertex_buf3 = mesh->data.genVertexBuffer();
                for(const auto& vgroup : mesh->data.triangle_groups){
                    index_buf3 = mesh->data.genIndexBuffer(vgroup.first);
                }
            }
        }
    }
    cout << "NRE VERTEX/INDEX BUFFERS " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    delete world3;//*/
    
    //ofstream myfile3;
    //myfile3.open ("csl_very_large_object_test_copy.csl");
    //myfile3 << c;
    //myfile3.close();
    
    //SDL_Delay(6000);
    
    return 0;
}
