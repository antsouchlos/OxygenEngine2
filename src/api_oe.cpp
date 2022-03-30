#include <OE/api_oe.h>
#include <OE/math_oe.h>

using namespace oe;
using namespace std;

OE_TaskManager* oe::OE_Main                      = nullptr;
bool            oe::preinit::use_legacy_renderer = false;

size_t oe::init() {
    OE_Main = new OE_TaskManager();
    return OE_Main->Init("Oxygen Engine Test", 800, 600, false, oe::preinit::use_legacy_renderer);
}
size_t oe::init(std::string title, bool fullscreen) {
    OE_Main = new OE_TaskManager();
    return OE_Main->Init(title, 800, 600, fullscreen, oe::preinit::use_legacy_renderer);
}
size_t oe::init(std::string title, int x, int y, bool fullscreen) {
    OE_Main = new OE_TaskManager();
    return OE_Main->Init(title, x, y, fullscreen, oe::preinit::use_legacy_renderer);
}
void oe::step() {
    OE_API_Helpers::checkIfInit();
    // cout << "running step" << endl;
    OE_Main->Step();
}
void oe::start() {
    OE_API_Helpers::checkIfInit();
    OE_Main->Start();
}
void oe::destroy() {
    OE_API_Helpers::checkIfInit();

    if (!oe::is_done()) {
        oe::finish();
        oe::step();
    }

    OE_Main->Destroy();
    delete OE_Main;
    OE_Main = nullptr;
}
bool oe::is_done() {
    OE_API_Helpers::checkIfInit();
    return OE_Main->done;
}
// ?? Where do i even need this ??? UPDATE: Now I remember
void oe::finish() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.done = true;
}

// size_t OE_InitFromFile(std::string); //TODO

//------------------------BLOCK-------------------------//
// The API functions inside this block are the only ones that are
// safe to be called on an unsynchronized thread

void oe::remove_task(std::string task) {
    OE_API_Helpers::checkIfInit();
    OE_Main->RemoveTask(task);
}

void oe::remove_task(std::string task, std::string thread) {
    OE_API_Helpers::checkIfInit();
    OE_Main->RemoveTask(task, thread);
}

void oe::broadcast_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.broadcastIEvent(name);
}
void oe::create_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.createUserEvent(name);
}

size_t oe::get_event_activations(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window->event_handler.getEventActivations(name);
}

size_t oe::get_event_counter(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window->event_handler.getEventCounter(name);
}

bool oe::is_key_just_pressed(std::string key) {
    OE_API_Helpers::checkIfInit();
    return oe::get_event_activations(key + "+") > 0;
}

bool oe::is_key_just_released(std::string key) {
    OE_API_Helpers::checkIfInit();
    return oe::get_event_activations(key + "-") > 0;
}

bool oe::is_key_held(std::string key) {
    OE_API_Helpers::checkIfInit();
    return oe::get_event_activations(key) > 0;
}

bool oe::is_key_pressed(std::string key) {
    OE_API_Helpers::checkIfInit();
    return (oe::get_event_activations(key) > 0) || (oe::get_event_activations(key + "+") > 0);
}

bool oe::is_mouse_moved() {
    return oe::get_event_activations("mouse-motion") > 0;
}

int oe::get_delta_mouse_x() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.lockMutex();
    int output = OE_Main->window->event_handler.get_mouse_delta_x();
    OE_Main->window->event_handler.unlockMutex();
    return output;
}

int oe::get_delta_mouse_y() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.lockMutex();
    int output = OE_Main->window->event_handler.get_mouse_delta_y();
    OE_Main->window->event_handler.unlockMutex();
    return output;
}

int oe::get_mouse_x() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.lockMutex();
    int output = OE_Main->window->event_handler.get_mouse_x();
    OE_Main->window->event_handler.unlockMutex();
    return output;
}

int oe::get_mouse_y() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.lockMutex();
    int output = OE_Main->window->event_handler.get_mouse_y();
    OE_Main->window->event_handler.unlockMutex();
    return output;
}

void oe::destroy_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window->event_handler.lockMutex();
    OE_Main->window->event_handler.destroyIEvent(name);
    OE_Main->window->event_handler.unlockMutex();
}

void oe::pause(int x) {
    OE_API_Helpers::checkIfInit();
    OE_Main->pause(x);
}

bool oe::is_mouse_locked() {
    return OE_Main->window->getMouseLockedState();
}

void oe::mouse_lock() {

    oe::create_event("mouse-lock");
    oe::set_event_func("mouse-lock", &OE_API_Helpers::manage_mouse);
    oe::broadcast_event("mouse-lock");
}
void oe::mouse_unlock() {
    oe::create_event("mouse-unlock");
    oe::set_event_func("mouse-unlock", &OE_API_Helpers::manage_mouse);
    oe::broadcast_event("mouse-unlock");
}

//------------------------BLOCK-------------------------//

OE_Task oe::get_task_info(std::string thread, std::string task) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->GetTaskInfo(thread, task);
}

void oe::create_new_thread(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->CreateNewThread(name);
}

/** API functions for manipulating objects and basic types
 * to be vastly extended when the physics engine comes
 */

std::size_t oe::get_object_id(std::string name) {
    OE_World::objectsList.lockMutex();
    std::size_t output = OE_World::objectsList.name2id[name];
    OE_World::objectsList.unlockMutex();
    return output;
}

std::string oe::get_object_name(std::size_t id) {
    OE_World::objectsList.lockMutex();
    std::string output;
    output = OE_World::objectsList.get_name(id);
    OE_World::objectsList.unlockMutex();
    return output;
}

std::set<std::size_t> oe::get_scene_objects(std::size_t id) {

    std::set<std::size_t> output;

    auto scene = OE_World::scenesList[id];
    if (scene.is_valid()) {
        scene.p_->lockMutex();
        output = scene.p_->objects;
        scene.p_->unlockMutex();
    }
    else {
        // TODO output a warning
    }
    return output;
}

OE_Vec3 oe::get_object_pos(std::size_t id) {

    OE_Vec3 output;

    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        output[0] = object.p_->current_state.pos_x;
        output[1] = object.p_->current_state.pos_y;
        output[2] = object.p_->current_state.pos_z;
        object.p_->unlockMutex();
    }
    return output;
}

OE_Quat oe::get_object_rot(std::size_t id) {

    OE_Quat output = OE_Quat();

    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        output = object.p_->GetRot();
        object.p_->unlockMutex();
    }
    return output;
}

std::set<std::size_t> oe::get_scene_objects(std::string name) {

    std::set<std::size_t> output;

    auto scene = OE_World::scenesList[name];
    if (scene.is_valid()) {
        scene.p_->lockMutex();
        output = scene.p_->objects;
        scene.p_->unlockMutex();
    }
    else {
        // TODO output a warning
    }
    return output;
}

OE_Vec3 oe::get_object_pos(std::string name) {

    OE_Vec3 output;

    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        output[0] = object.p_->current_state.pos_x;
        output[1] = object.p_->current_state.pos_y;
        output[2] = object.p_->current_state.pos_z;
        object.p_->unlockMutex();
    }
    return output;
}

OE_Quat oe::get_object_rot(std::string name) {

    OE_Quat output;

    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        output = object.p_->GetRot();
        object.p_->unlockMutex();
    }
    return output;
}

void oe::set_object_pos(std::size_t id, OE_Vec3 pos) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.pos_x = pos.x;
        object.p_->current_state.pos_y = pos.y;
        object.p_->current_state.pos_z = pos.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_rot(std::size_t id, OE_Quat rot) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(rot);
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_rot(std::size_t id, OE_Vec4 rot) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_pos(std::size_t id, OE_Vec3 pos) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.pos_x = object.p_->current_state.pos_x + pos.x;
        object.p_->current_state.pos_y = object.p_->current_state.pos_y + pos.y;
        object.p_->current_state.pos_z = object.p_->current_state.pos_z + pos.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_rot(std::size_t id, OE_Quat rot) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(object.p_->GetRot() * rot);
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_rot(std::size_t id, OE_Vec4 rot) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(object.p_->GetRot() * OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_pos(std::string name, OE_Vec3 pos) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.pos_x = pos.x;
        object.p_->current_state.pos_y = pos.y;
        object.p_->current_state.pos_z = pos.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_rot(std::string name, OE_Quat rot) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(rot);
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_rot(std::string name, OE_Vec4 rot) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_pos(std::string name, OE_Vec3 pos) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.pos_x = object.p_->current_state.pos_x + pos.x;
        object.p_->current_state.pos_y = object.p_->current_state.pos_y + pos.y;
        object.p_->current_state.pos_z = object.p_->current_state.pos_z + pos.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_rot(std::string name, OE_Quat rot) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(object.p_->GetRot() * rot);
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_rot(std::string name, OE_Vec4 rot) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(object.p_->GetRot() * OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_global_rot(std::size_t id, OE_Quat rot) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(rot * object.p_->GetRot());
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_global_rot(std::size_t id, OE_Vec4 rot) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]) * object.p_->GetRot());
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_global_rot(std::string name, OE_Quat rot) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(rot * object.p_->GetRot());
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_global_rot(std::string name, OE_Vec4 rot) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]) * object.p_->GetRot());
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_local_pos(std::size_t id, OE_Vec3 pos) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        OE_Mat3x3 rot_matrix           = object.p_->GetModelMatrix();
        auto      local_pos            = rot_matrix * pos;
        object.p_->current_state.pos_x = object.p_->current_state.pos_x + local_pos.x;
        object.p_->current_state.pos_y = object.p_->current_state.pos_y + local_pos.y;
        object.p_->current_state.pos_z = object.p_->current_state.pos_z + local_pos.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_local_pos(std::string name, OE_Vec3 pos) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        OE_Mat3x3 rot_matrix           = object.p_->GetModelMatrix();
        auto      local_pos            = rot_matrix * pos;
        object.p_->current_state.pos_x = object.p_->current_state.pos_x + local_pos.x;
        object.p_->current_state.pos_y = object.p_->current_state.pos_y + local_pos.y;
        object.p_->current_state.pos_z = object.p_->current_state.pos_z + local_pos.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_scale(std::size_t id, OE_Vec3 sca) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.sca_x = sca.x;
        object.p_->current_state.sca_y = sca.y;
        object.p_->current_state.sca_z = sca.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_scale(std::string name, OE_Vec3 sca) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.sca_x = sca.x;
        object.p_->current_state.sca_y = sca.y;
        object.p_->current_state.sca_z = sca.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_scale(std::size_t id, OE_Vec3 sca) {
    auto object = OE_World::objectsList[id];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.sca_x = object.p_->current_state.sca_x + sca.x;
        object.p_->current_state.sca_y = object.p_->current_state.sca_y + sca.y;
        object.p_->current_state.sca_z = object.p_->current_state.sca_z + sca.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_scale(std::string name, OE_Vec3 sca) {
    auto object = OE_World::objectsList[name];
    if (object.is_valid()) {
        object.p_->lockMutex();
        object.p_->current_state.sca_x = object.p_->current_state.sca_x + sca.x;
        object.p_->current_state.sca_y = object.p_->current_state.sca_y + sca.y;
        object.p_->current_state.sca_z = object.p_->current_state.sca_z + sca.z;
        object.p_->unlockMutex();
        object.flag_as_changed();
    }
}


/** API functions to control the renderer
 *  These should work for ANY renderer
 */

void oe::restart_renderer() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_mutex.lockMutex();
    if (OE_Main->window != nullptr) OE_Main->window->restart_renderer = true;
    OE_Main->window_mutex.unlockMutex();
}

void oe::set_shading_mode(OE_RENDERER_SHADING_MODE shading_mode) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->lockMutex();
        OE_Main->renderer->shading_mode = shading_mode;
        OE_Main->renderer->unlockMutex();
    }
    OE_Main->renderer_mutex.unlockMutex();
    oe::restart_renderer();
}

OE_RENDERER_SHADING_MODE oe::get_shading_mode() {
    OE_API_Helpers::checkIfInit();
    OE_RENDERER_SHADING_MODE output = OE_RENDERER_REGULAR_SHADING;
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->lockMutex();
        output = OE_Main->renderer->shading_mode;
        OE_Main->renderer->unlockMutex();
    }
    OE_Main->renderer_mutex.unlockMutex();
    return output;
}

void oe::render_wireframe(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->use_wireframe = value;
    }
    OE_Main->renderer_mutex.unlockMutex();
}

void oe::toggle_wireframe_rendering() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->lockMutex();
        if (OE_Main->renderer->use_wireframe)
            OE_Main->renderer->use_wireframe = false;
        else
            OE_Main->renderer->use_wireframe = true;
        OE_Main->renderer->unlockMutex();
    }
    OE_Main->renderer_mutex.unlockMutex();
}

void oe::render_bounding_boxes(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->render_bounding_boxes = value;
    }
    OE_Main->renderer_mutex.unlockMutex();
}
void oe::toggle_bounding_boxes_rendering() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->lockMutex();
        if (OE_Main->renderer->render_bounding_boxes)
            OE_Main->renderer->render_bounding_boxes = false;
        else
            OE_Main->renderer->render_bounding_boxes = true;
        OE_Main->renderer->unlockMutex();
    }
    OE_Main->renderer_mutex.unlockMutex();
}

void oe::render_bounding_spheres(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->render_bounding_spheres = value;
    }
    OE_Main->renderer_mutex.unlockMutex();
}
void oe::toggle_bounding_spheres_rendering() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->lockMutex();
        if (OE_Main->renderer->render_bounding_spheres)
            OE_Main->renderer->render_bounding_spheres = false;
        else
            OE_Main->renderer->render_bounding_spheres = true;
        OE_Main->renderer->unlockMutex();
    }
    OE_Main->renderer_mutex.unlockMutex();
}

void oe::render_HDR(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->use_HDR = value;
    }
    OE_Main->renderer_mutex.unlockMutex();
}
void oe::toggle_render_HDR() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex.lockMutex();
    if (OE_Main->renderer != nullptr) {
        OE_Main->renderer->lockMutex();
        if (OE_Main->renderer->use_HDR)
            OE_Main->renderer->use_HDR = false;
        else
            OE_Main->renderer->use_HDR = true;
        OE_Main->renderer->unlockMutex();
    }
    OE_Main->renderer_mutex.unlockMutex();
}
