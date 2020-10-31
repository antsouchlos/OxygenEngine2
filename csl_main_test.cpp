
#include <OE_API.h>

using namespace std;
using namespace OE;

int update_camera_pos(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_z = object.second->current_state.pos_z - 0.1f;
            }
        }
    }
    return 0;
}

int update_camera_pos_neg(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_z = object.second->current_state.pos_z + 0.1f;
            }
        }
    }
    return 0;
}

int update_camera_pos_y(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_x = object.second->current_state.pos_x - 0.1f;
            }
        }
    }
    return 0;
}

int update_camera_pos_neg_y(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_x = object.second->current_state.pos_x + 0.1f;
            }
        }
    }
    return 0;
}

int OnLoadObject(void*, OE_Task* event_task, string event_name){
    cout << "SUCCESSFULLY loaded '" << event_name << "'" << endl;
    OE_SetEventFunc("keyboard-w", &update_camera_pos, nullptr);
    OE_SetEventFunc("keyboard-s", &update_camera_pos_neg, nullptr);
    OE_SetEventFunc("keyboard-a", &update_camera_pos_y, nullptr);
    OE_SetEventFunc("keyboard-d", &update_camera_pos_neg_y, nullptr);
    return 0;
}

int main(){
        
    //OE_TaskManager taskMgr = OE_TaskManager();
    
    //taskMgr.Init();
    OE_Init("Oxygen Engine Demo", 1280, 720, false);
    OE_Pause(20);
    OE_LoadWorld("monkeys.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("fe_export_test.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("csl_very_large_object_test.csl", &OnLoadObject, nullptr);
    //taskMgr.Start();
    OE_Start();
    
    /*CSL_Interpreter interpreter;
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
