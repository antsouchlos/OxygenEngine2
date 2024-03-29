#include <OE/global_variables.h>
#include <OE/types/light.h>

using namespace std;

std::vector<float> OE_RGBColor::to_arr() const {
    return {r, g, b};
}

OE_Light::OE_Light() : OE_Object() {

    this->light_type = 0;
    this->fov        = 0.0f;
    this->range      = 0.0f;
    this->intensity  = 0.0f;
}


OE_Light::~OE_Light() {
}

std::vector<float> OE_Light::GetLightGPUData() {
    std::vector<float> output;

    // point_light
    if (this->light_type == 1) {
        output.push_back(this->current_state.pos_x);
        output.push_back(this->current_state.pos_y);
        output.push_back(this->current_state.pos_z);
        output.push_back(this->intensity);

        output.push_back(this->color.r);
        output.push_back(this->color.g);
        output.push_back(this->color.b);
        output.push_back(this->range);
    }
    // sun light (directional)
    else if (this->light_type == 2) {
    }
    else {
        // UNDEFINED
    }


    return output;
}

OE_OBJECT_TYPE OE_Light::getType() const {
    return OE_OBJECT_LIGHT;
}

string OE_Light::to_str() const {
    string output = outputTypeTag(
        "Light", {{"name", "\"" + oe::objects_list.get_name(this->id) + "\""}, {"visible", convert((int)visible)}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;

    output.append(outputList("current_state", this->current_state.to_arr()));
    output.append("\n");

    output.append(outputVar("parent", "\"" + oe::objects_list.get_name(this->parent) + "\""));
    output.append("\n");

    output.append(outputVar("parent_type", convert(this->parent_type)));
    output.append("\n");

    vector<string> object_strs;
    for (const auto& x : this->objects) {
        object_strs.push_back("\"" + oe::objects_list.get_name(x) + "\"");
    }

    if (object_strs.size() != 0) {
        output.append(outputList("objects", object_strs));
        output.append("\n");
    }

    output.append(outputList("color", this->color.to_arr()));
    output.append("\n");

    output.append(outputVar("intensity", convert(this->intensity)));
    output.append("\n");

    output.append(outputVar("fov", convert(this->fov)));
    output.append("\n");

    output.append(outputVar("light_type", convert(this->light_type)));
    output.append("\n");

    output.append(outputVar("range", convert(this->range)));
    output.append("\n");

    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Light"));
    return output;
}
