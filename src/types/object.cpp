#include <OE/math_oe.h>
#include <OE/types/object.h>
#include <OE/types/world.h>

using namespace std;

std::vector<float> OE_ObjectData::to_arr() const {
    return {pos_x, pos_y, pos_z, rot_w, rot_x, rot_y, rot_z, sca_x, sca_y, sca_z};
}


std::atomic<size_t> OE_Object::current_id(0);

OE_Object::OE_Object() {

    this->parent      = 0;
    this->parent_type = 0;
    this->visible     = true;

    this->id = ++OE_Object::current_id;
}


OE_Object::OE_Object(const string& name) {

    this->parent      = 0;
    this->parent_type = 0;
    this->visible     = true;

    this->id = ++OE_Object::current_id;
}

OE_Object::~OE_Object() {
}

string OE_Object::to_str() const {
    return "NOTHING";
}

OE_OBJECT_TYPE OE_Object::getType() const {
    return OE_OBJECT_UNDEFINED;
}

void OE_Object::SetRot(OE_Quat quat) {
    this->current_state.rot_x = quat.x;
    this->current_state.rot_y = quat.y;
    this->current_state.rot_z = quat.z;
    this->current_state.rot_w = quat.w;
}

OE_Quat OE_Object::GetRot() {
    return OE_Quat(current_state.rot_w, current_state.rot_x, current_state.rot_y, current_state.rot_z);
}

OE_Mat4x4 OE_Object::GetModelMatrix() {

    OE_Mat4x4 model_mat(1.0f);

    // handle scale
    OE_Vec3 scale_vec = OE_Vec3(this->current_state.sca_x, this->current_state.sca_y, this->current_state.sca_z);

    // handle rotation
    OE_Vec4 nor_quat = OE_Normalize(
        OE_Vec4(this->current_state.rot_w, this->current_state.rot_x, this->current_state.rot_y, this->current_state.rot_z));
    OE_Quat rot_quat = OE_Quat(nor_quat[0], nor_quat[1], nor_quat[2], nor_quat[3]);

    // handle translation
    OE_Vec3 translation_vec = OE_Vec3(this->current_state.pos_x, this->current_state.pos_y, this->current_state.pos_z);

    // first scale, then rotate, finally translate.
    // It MUST happen in this order
    // or else expect funny results
    model_mat = OE_Translate(model_mat, translation_vec) * OE_Quat2Mat4x4(rot_quat) * OE_Scale(model_mat, scale_vec);
    return model_mat;
}

OE_Mat4x4 OE_Object::GetViewMatrix() {

    OE_Mat4x4 view_mat(1.0f);
    OE_Vec4   nor_quat = OE_Normalize(
          OE_Vec4(this->current_state.rot_w, -this->current_state.rot_x, -this->current_state.rot_y, -this->current_state.rot_z));

    OE_Quat rot_quat        = OE_Quat(nor_quat[0], nor_quat[1], nor_quat[2], nor_quat[3]);
    OE_Vec3 translation_vec = OE_Vec3(-this->current_state.pos_x, -this->current_state.pos_y, -this->current_state.pos_z);
    view_mat                = OE_Quat2Mat4x4(rot_quat) * OE_Translate(view_mat, translation_vec);
    return view_mat;
}
