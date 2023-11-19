#include <OE/api_oe.h>
#include <OE/math_oe.h>
#include <OE/types/libs_oe.h>

using namespace oe;
using namespace std;

oe::task_manager_t*      oe::OE_Main                        = nullptr;
oe::renderer_init_info   oe::preinit::renderer_parameters   = oe::renderer_init_info();
oe::winsys_init_info     oe::preinit::winsys_parameters     = oe::winsys_init_info();
oe::physics_init_info    oe::preinit::physics_parameters    = oe::physics_init_info();
oe::networking_init_info oe::preinit::networking_parameters = oe::networking_init_info();

void oe::preinit::request_gles2() {
    oe::preinit::winsys_parameters.requested_backend = nre::gpu::GLES2;
}

void oe::preinit::request_gles31() {
    oe::preinit::winsys_parameters.requested_backend = nre::gpu::GLES;
}

size_t oe::init() {
    OE_Main = new oe::task_manager_t();
    return OE_Main->init("Oxygen Engine Test", 800, 600, false, preinit::renderer_parameters, preinit::winsys_parameters,
                         preinit::physics_parameters, oe::preinit::networking_parameters);
}
size_t oe::init(std::string title, bool fullscreen) {
    OE_Main = new oe::task_manager_t();
    return OE_Main->init(title, 800, 600, fullscreen, preinit::renderer_parameters, preinit::winsys_parameters,
                         preinit::physics_parameters, oe::preinit::networking_parameters);
}
size_t oe::init(std::string title, int x, int y, bool fullscreen) {
    OE_Main = new oe::task_manager_t();
    return OE_Main->init(title, x, y, fullscreen, preinit::renderer_parameters, preinit::winsys_parameters,
                         preinit::physics_parameters, oe::preinit::networking_parameters);
}
void oe::step() {
    OE_API_Helpers::checkIfInit();
    // cout << "running step" << endl;
    OE_Main->step();
}
void oe::start() {
    OE_API_Helpers::checkIfInit();
    OE_Main->start();
}
void oe::destroy() {
    OE_API_Helpers::checkIfInit();

    if (!oe::is_done()) {
        oe::finish();
        oe::step();
    }

    OE_Main->destroy();
    delete OE_Main;
    OE_Main = nullptr;
}
bool oe::is_done() {
    OE_API_Helpers::checkIfInit();
    return OE_Main->is_done();
}
// ?? Where do i even need this ??? UPDATE: Now I remember
void oe::finish() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.set_done(true);
}

// size_t OE_InitFromFile(std::string); //TODO

//------------------------BLOCK-------------------------//
// The API functions inside this block are the only ones that are
// safe to be called on an unsynchronized thread

void oe::remove_task(std::string task) {
    OE_API_Helpers::checkIfInit();
    OE_Main->remove_task(task);
}

void oe::remove_task(std::string task, std::string thread) {
    OE_API_Helpers::checkIfInit();
    OE_Main->remove_task(task, thread);
}

void oe::broadcast_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.broadcast_event(name);
}
std::size_t oe::create_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window_->event_handler_.create_user_event(name);
}
std::size_t oe::create_internal_physics_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window_->event_handler_.create_physics_event(name);
}
std::size_t oe::create_internal_renderer_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window_->event_handler_.create_renderer_event(name);
}
std::size_t oe::create_internal_network_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window_->event_handler_.create_network_event(name);
}
void oe::broadcast_event(std::size_t id) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.broadcast_event(id);
}

std::size_t oe::get_event_id(std::string name) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window_->event_handler_.get_event_id(name);
}

std::string oe::get_event_name(std::size_t id) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->window_->event_handler_.get_event_name(id);
}

size_t oe::get_event_activations(std::string name) {
    OE_API_Helpers::checkIfInit();
    size_t event_id = OE_Main->window_->event_handler_.get_event_id(name);
    return OE_Main->window_->event_handler_.get_event_activations(event_id);
}

size_t oe::get_event_counter(std::string name) {
    OE_API_Helpers::checkIfInit();
    size_t event_id = OE_Main->window_->event_handler_.get_event_id(name);
    return OE_Main->window_->event_handler_.get_event_counter(event_id);
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
    OE_Main->window_->event_handler_.lockMutex();
    int output = OE_Main->window_->event_handler_.get_mouse_delta_x();
    OE_Main->window_->event_handler_.unlockMutex();
    return output;
}

int oe::get_delta_mouse_y() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.lockMutex();
    int output = OE_Main->window_->event_handler_.get_mouse_delta_y();
    OE_Main->window_->event_handler_.unlockMutex();
    return output;
}

int oe::get_mouse_x() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.lockMutex();
    int output = OE_Main->window_->event_handler_.get_mouse_x();
    OE_Main->window_->event_handler_.unlockMutex();
    return output;
}

int oe::get_mouse_y() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.lockMutex();
    int output = OE_Main->window_->event_handler_.get_mouse_y();
    OE_Main->window_->event_handler_.unlockMutex();
    return output;
}

void oe::destroy_event(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_->event_handler_.lockMutex();
    size_t event_id = OE_Main->window_->event_handler_.get_event_id(name);
    OE_Main->window_->event_handler_.destroy_event(event_id);
    OE_Main->window_->event_handler_.unlockMutex();
}

void oe::pause(int x) {
    OE_API_Helpers::checkIfInit();
    OE_Main->pause(x);
}

bool oe::is_mouse_locked() {
    return OE_Main->window_->is_mouse_locked();
}

void oe::mouse_lock() {
    oe::broadcast_event("mouse-lock");
}

void oe::mouse_unlock() {
    oe::broadcast_event("mouse-unlock");
}

//------------------------BLOCK-------------------------//
oe::task_info_t oe::get_task_info(std::string thread, std::string task) {
    OE_API_Helpers::checkIfInit();
    return OE_Main->get_task_info(thread, task);
}

void oe::create_new_thread(std::string name) {
    OE_API_Helpers::checkIfInit();
    OE_Main->create_new_thread(name);
}

/** API functions for manipulating objects and basic types
 * to be vastly extended when the physics engine comes
 */

std::size_t oe::get_object_id(std::string name) {
    return oe::objects_list.get_id(name);
}

std::string oe::get_object_name(std::size_t id) {
    return oe::objects_list.get_name(id);
}

std::set<std::size_t> oe::get_scene_objects(std::size_t id) {

    std::set<std::size_t> output;

    auto scene = oe::scenes_list[id];
    if (scene.is_valid()) {
        scene->lockMutex();
        output = scene->objects;
        scene->unlockMutex();
    }
    else {
        // TODO output a warning
    }
    return output;
}

OE_Vec3 oe::get_object_pos(std::size_t id) {

    OE_Vec3 output;

    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        output[0] = object->current_state.pos_x;
        output[1] = object->current_state.pos_y;
        output[2] = object->current_state.pos_z;
        object->unlockMutex();
    }
    return output;
}

OE_Quat oe::get_object_rot(std::size_t id) {

    OE_Quat output = OE_Quat();

    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        output = object->GetRot();
        object->unlockMutex();
    }
    return output;
}

void oe::set_object_pos(std::size_t id, OE_Vec3 pos) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->current_state.pos_x = pos.x;
        object->current_state.pos_y = pos.y;
        object->current_state.pos_z = pos.z;
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_rot(std::size_t id, OE_Quat rot) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->SetRot(rot);
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_rot(std::size_t id, OE_Vec4 rot) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_pos(std::size_t id, OE_Vec3 pos) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->current_state.pos_x = object->current_state.pos_x + pos.x;
        object->current_state.pos_y = object->current_state.pos_y + pos.y;
        object->current_state.pos_z = object->current_state.pos_z + pos.z;
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_rot(std::size_t id, OE_Quat rot) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->SetRot(object->GetRot() * rot);
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_rot(std::size_t id, OE_Vec4 rot) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->SetRot(object->GetRot() * OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_global_rot(std::size_t id, OE_Quat rot) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->SetRot(rot * object->GetRot());
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_global_rot(std::size_t id, OE_Vec4 rot) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]) * object->GetRot());
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_local_pos(std::size_t id, OE_Vec3 pos) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        OE_Mat3x3 rot_matrix        = object->GetModelMatrix();
        auto      local_pos         = rot_matrix * pos;
        object->current_state.pos_x = object->current_state.pos_x + local_pos.x;
        object->current_state.pos_y = object->current_state.pos_y + local_pos.y;
        object->current_state.pos_z = object->current_state.pos_z + local_pos.z;
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::set_object_scale(std::size_t id, OE_Vec3 sca) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->current_state.sca_x = sca.x;
        object->current_state.sca_y = sca.y;
        object->current_state.sca_z = sca.z;
        object->unlockMutex();
        object.flag_as_changed();
    }
}

void oe::change_object_scale(std::size_t id, OE_Vec3 sca) {
    auto object = oe::objects_list[id];
    if (object.is_valid()) {
        object->lockMutex();
        object->current_state.sca_x = object->current_state.sca_x + sca.x;
        object->current_state.sca_y = object->current_state.sca_y + sca.y;
        object->current_state.sca_z = object->current_state.sca_z + sca.z;
        object->unlockMutex();
        object.flag_as_changed();
    }
}

/** API functions to control the renderer
 *  These should work for ANY renderer
 */

void oe::restart_renderer() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_mutex_.lockMutex();
    OE_Main->force_restart_renderer();
    OE_Main->window_mutex_.unlockMutex();
}

void oe::set_shading_mode(oe::RENDERER_SHADING_MODE shading_mode) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.shading_mode = shading_mode;
    OE_Main->renderer_mutex_.unlockMutex();
    oe::restart_renderer();
}

oe::RENDERER_SHADING_MODE oe::get_shading_mode() {
    OE_API_Helpers::checkIfInit();
    oe::RENDERER_SHADING_MODE output;
    OE_Main->renderer_mutex_.lockMutex();
    output = OE_Main->renderer_info_.shading_mode;
    OE_Main->renderer_mutex_.unlockMutex();
    return output;
}

void oe::render_wireframe(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.use_wireframe = value;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::toggle_wireframe_rendering() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.use_wireframe)
        OE_Main->renderer_info_.use_wireframe = false;
    else
        OE_Main->renderer_info_.use_wireframe = true;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::render_bounding_boxes(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.render_bounding_boxes = value;
    OE_Main->renderer_mutex_.unlockMutex();
}
void oe::toggle_bounding_boxes_rendering() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.render_bounding_boxes)
        OE_Main->renderer_info_.render_bounding_boxes = false;
    else
        OE_Main->renderer_info_.render_bounding_boxes = true;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::render_bounding_spheres(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.render_bounding_spheres = value;
    OE_Main->renderer_mutex_.unlockMutex();
}
void oe::toggle_bounding_spheres_rendering() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.render_bounding_spheres)
        OE_Main->renderer_info_.render_bounding_spheres = false;
    else
        OE_Main->renderer_info_.render_bounding_spheres = true;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::render_HDR(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.use_hdr = value;
    OE_Main->renderer_mutex_.unlockMutex();
}
void oe::toggle_render_HDR() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.use_hdr)
        OE_Main->renderer_info_.use_hdr = false;
    else
        OE_Main->renderer_info_.use_hdr = true;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::render_software_vertex_shaders(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.use_software_vertex_shaders = value;
    OE_Main->renderer_mutex_.unlockMutex();
}
void oe::toggle_software_vertex_shaders() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.use_software_vertex_shaders)
        OE_Main->renderer_info_.use_software_vertex_shaders = false;
    else
        OE_Main->renderer_info_.use_software_vertex_shaders = true;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::render_z_prepass(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.use_z_prepass = value;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::toggle_render_z_prepass() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.use_z_prepass)
        OE_Main->renderer_info_.use_z_prepass = false;
    else
        OE_Main->renderer_info_.use_z_prepass = true;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::set_renderer_sanity_checks(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    OE_Main->renderer_info_.sanity_checks = value;
    OE_Main->renderer_mutex_.unlockMutex();
}

void oe::toggle_renderer_sanity_checks() {
    OE_API_Helpers::checkIfInit();
    OE_Main->renderer_mutex_.lockMutex();
    if (OE_Main->renderer_info_.sanity_checks)
        OE_Main->renderer_info_.sanity_checks = false;
    else
        OE_Main->renderer_info_.sanity_checks = true;
    OE_Main->renderer_mutex_.unlockMutex();
}


void oe::set_window_title(std::string title) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_mutex_.lockMutex();
    OE_Main->window_info_.title = title;
    OE_Main->window_mutex_.unlockMutex();
}

std::string oe::get_window_title() {
    OE_API_Helpers::checkIfInit();
    std::string output;
    OE_Main->window_mutex_.lockMutex();
    output = OE_Main->window_info_.title;
    OE_Main->window_mutex_.unlockMutex();
    return output;
}

void oe::toggle_window_fullscreen() {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_mutex_.lockMutex();
    if (OE_Main->window_info_.use_fullscreen)
        OE_Main->window_info_.use_fullscreen = false;
    else
        OE_Main->window_info_.use_fullscreen = true;
    OE_Main->window_mutex_.unlockMutex();
}

void oe::set_window_fullscreen(bool value) {
    OE_API_Helpers::checkIfInit();
    OE_Main->window_mutex_.lockMutex();
    OE_Main->window_info_.use_fullscreen = value;
    OE_Main->window_mutex_.unlockMutex();
}
