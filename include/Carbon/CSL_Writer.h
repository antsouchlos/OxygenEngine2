#ifndef CSL_WRITER_H_
#define CSL_WRITER_H_

#include "FE_Libs.h"

//inlcudes all types
#include "FE_World.h"

using namespace std;


class CSL_Writer {
public:
    string output(FE_World *world) {
		outputStr = "";

		outputWorld(world);

		return outputStr;
	}
private:
	//the return - variable of 'output'
	string outputStr;

	//specifies the indentation
	int indent = 0;

	//functions that generate the output for every type and add it to 'outputStr'

	void outputWorld(FE_World *world) {
		outputStr = "<World name=\"" + world->name + "\" loaded_scene=\"" + world->loaded_scene + "\">\n";

		indent++;

		for (auto& scene : world->scenes)
			outputScene(*scene);

		indent--;
		outputStr += "</World>\n";
	}

	void outputScene(FE_Scene& scene) {
		outputStr += genIndent() + "<Scene name=\"" + scene.name + "\">\n";
		indent++;

		outputStr += genIndent() + "gravity = " + convert(scene.gravity) + "\n";
		outputStr += genIndent() + "gravityAxis = \"" + scene.gravityAxis + "\"\n";
		outputStr += genIndent() + "groups = {";
		for (auto& group : scene.groups) outputStr += "\"" + group + "\"; ";
		outputStr = outputStr.substr(0, outputStr.size()-2);
		outputStr += "}\n\n";

		for (auto& camera : scene.cameras)
			outputCamera(*camera);

		for (auto& light : scene.lights)
			outputLight(*light);

		for (auto& interaction : scene.interactions)
			outputInteraction(*interaction);

		for (auto& mesh : scene.objects)
			outputMesh(*mesh);

		for (auto& texture : scene.textures)
			outputTexture(*texture);

		for (auto& material : scene.materials)
			outputMaterial(*material);

		for (auto& tcm : scene.textureCombineModes)
			outputTCM(*tcm);

		indent--;
		outputStr += genIndent() + "</Scene>\n\n";

	}

	void outputCamera(FE_Camera& camera) {
		outputStr += genIndent() + "<Camera name = \"" + camera.name +"\">\n";
		indent++;

		//camera variables
		outputStr += genIndent() + "aspect_ratio = " + convert(camera.aspect_ratio) + "\n";
		outputStr += genIndent() + "field_of_view = " + convert(camera.field_of_view) +  "\n";
		outputStr += genIndent() + "near = " + convert(camera.near) + "\n";
		outputStr += genIndent() + "far = " + convert(camera.far) + "\n\n";

		//object variables
		outputStr += genIndent() + "pos = {" + convert(camera.pos.x) + "; " + convert(camera.pos.y) + "; " + convert(camera.pos.z) + "; " + convert(camera.pos.w) + "; " + convert(camera.pos.axis_angle) + "}\n";
		outputStr += genIndent() + "rot = {" + convert(camera.rot.x) + "; " + convert(camera.rot.y) + "; " + convert(camera.rot.z) + "; " + convert(camera.rot.w) + "; " + convert(camera.rot.axis_angle) + "}\n";
		/*
		outputStr += genIndent() + "quat_rot = {" + convert(camera.quat_rot.rad) + "; " + convert(camera.quat_rot.x) + "; " + convert(camera.quat_rot.y) + "; " + convert(camera.quat_rot.z) + "; " + convert(camera.quat_rot.quatted) + "}\n";
		outputStr += genIndent() + "delta_pos = {" + convert(camera.delta_pos.x) + "; " + convert(camera.delta_pos.y) + "; " + convert(camera.delta_pos.z) + "; " + convert(camera.delta_pos.w) + "; " + convert(camera.delta_pos.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_rot = {" + convert(camera.delta_rot.x) + "; " + convert(camera.delta_rot.y) + "; " + convert(camera.delta_rot.z) + "; " + convert(camera.delta_rot.w) + "; " + convert(camera.delta_rot.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_sca = {" + convert(camera.delta_sca.x) + "; " + convert(camera.delta_sca.y) + "; " + convert(camera.delta_sca.z) + "; " + convert(camera.delta_sca.w) + "; " + convert(camera.delta_sca.axis_angle) + "}\n";
		outputStr += genIndent() + "delta_quat = {" + convert(camera.delta_quat.rad) + "; " + convert(camera.delta_quat.x) + "; " + convert(camera.delta_quat.y) + "; " + convert(camera.delta_quat.z) + "; " + convert(camera.delta_quat.quatted) + "}\n";

		outputStr += genIndent() + "rotation_type = " + convert(camera.rotation_type) + "\n";
		*/

		indent--;
		outputStr += genIndent() + "</Camera>\n\n";

	}

	void outputLight(FE_Light& light) {
                outputStr += genIndent() + "<Light name = \"" + light.name +"\">\n";
                indent++;

                //light variables
		outputStr += genIndent() + "light_type = \"" + light.light_type + "\"\n";
		outputStr += genIndent() + "value = " + convert(light.value) + "\n";
		outputStr += genIndent() + "fov = " + convert(light.fov) + "\n";
		outputStr += genIndent() + "range = " + convert(light.range) + "\n";
		outputStr += genIndent() + "color = {" + convert(light.color.x) + "; " + convert(light.color.y) + "; " + convert(light.color.z) + "; " + convert(light.color.w) + "; " + convert(light.color.axis_angle) + "}\n\n";

                //object variables
                outputStr += genIndent() + "pos = {" + convert(light.pos.x) + "; " + convert(light.pos.y) + "; " + convert(light.pos.z) + "; " + convert(light.pos.w) + "; " + convert(light.pos.axis_angle) + "}\n";
                outputStr += genIndent() + "rot = {" + convert(light.rot.x) + "; " + convert(light.rot.y) + "; " + convert(light.rot.z) + "; " + convert(light.rot.w) + "; " + convert(light.rot.axis_angle) + "}\n";
                /*
		outputStr += genIndent() + "sca = {" + convert(light.sca.x) + "; " + convert(light.sca.y) + "; " + convert(light.sca.z) + "; " + convert(light.sca.w) + "; " + convert(light.sca.axis_angle) + "}\n";
                outputStr += genIndent() + "quat_rot = {" + convert(light.quat_rot.rad) + "; " + convert(light.quat_rot.x) + "; " + convert(light.quat_rot.y) + "; " + convert(light.quat_rot.z) + "; " + convert(light.quat_rot.quatted) + "}\n";

                outputStr += genIndent() + "delta_pos = {" + convert(light.delta_pos.x) + "; " + convert(light.delta_pos.y) + "; " + convert(light.delta_pos.z) + "; " + convert(light.delta_pos.w) + "; " + convert(light.delta_pos.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_rot = {" + convert(light.delta_rot.x) + "; " + convert(light.delta_rot.y) + "; " + convert(light.delta_rot.z) + "; " + convert(light.delta_rot.w) + "; " + convert(light.delta_rot.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_sca = {" + convert(light.delta_sca.x) + "; " + convert(light.delta_sca.y) + "; " + convert(light.delta_sca.z) + "; " + convert(light.delta_sca.w) + "; " + convert(light.delta_sca.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_quat = {" + convert(light.delta_quat.rad) + "; " + convert(light.delta_quat.x) + "; " + convert(light.delta_quat.y) + "; " + convert(light.delta_quat.z) + "; " + convert(light.delta_quat.quatted) + "}\n";

                outputStr += genIndent() + "rotation_type = " + convert(light.rotation_type) + "\n";
		*/

                indent--;
                outputStr += genIndent() + "</Light>\n\n";

	}

        void outputInteraction(FE_Interaction& interaction) {
		outputStr += genIndent() + "<Interaction name=\"" + interaction.name + "\">\n";
		indent++;

		outputStr += genIndent() + "objectNames = {";
		for (auto& name : interaction.objectNames) outputStr += "\"" + name + "\"; ";
		outputStr = outputStr.substr(0, outputStr.size()-2);
		outputStr += "}\n\n";

		outputStr += genIndent() + "interaction = \"" + interaction.interaction + "\"\n";

		indent--;
		outputStr += genIndent() + "</Interaction>\n\n";
        }

        void outputMesh(FE_Mesh& mesh) {
                outputStr += genIndent() + "<Mesh name = \"" + mesh.name +"\">\n";
                indent++;

                //mesh variables
        /* Made necessary changes
         * for getting python and C++ Writers at the same level
         *
         */
		size_t vertex_id = 0;
		for (auto& vertex : mesh.vertices){
				outputVertex(*vertex, vertex_id);
				vertex_id++;
		}
		for (auto& vgroup : mesh.vertex_groups)
			outputVertexGroup(*vgroup);

		for (auto& animation : mesh.animations)
			outputAnimation(*animation);

		for (auto& polygon : mesh.polygons)
			outputPolygon(*polygon);

		outputPhysics(mesh.physics);

		outputStr += genIndent() + "lights = {";
		for (auto& light : mesh.lights) outputStr += "\"" + light + "\"; ";
		outputStr = outputStr.substr(0, outputStr.size()-2);
		outputStr += "}\n\n";

                //object variables
                outputStr += genIndent() + "pos = {" + convert(mesh.pos.x) + "; " + convert(mesh.pos.y) + "; " + convert(mesh.pos.z) + "; " + convert(mesh.pos.w) + "; " + convert(mesh.pos.axis_angle) + "}\n";
                outputStr += genIndent() + "rot = {" + convert(mesh.rot.x) + "; " + convert(mesh.rot.y) + "; " + convert(mesh.rot.z) + "; " + convert(mesh.rot.w) + "; " + convert(mesh.rot.axis_angle) + "}\n";
                outputStr += genIndent() + "sca = {" + convert(mesh.sca.x) + "; " + convert(mesh.sca.y) + "; " + convert(mesh.sca.z) + "; " + convert(mesh.sca.w) + "; " + convert(mesh.sca.axis_angle) + "}\n";
                /*
		outputStr += genIndent() + "quat_rot = {" + convert(mesh.quat_rot.rad) + "; " + convert(mesh.quat_rot.x) + "; " + convert(mesh.quat_rot.y) + "; " + convert(mesh.quat_rot.z) + "; " + convert(mesh.quat_rot.quatted) + "}\n";

                outputStr += genIndent() + "delta_pos = {" + convert(mesh.delta_pos.x) + "; " + convert(mesh.delta_pos.y) + "; " + convert(mesh.delta_pos.z) + "; " + convert(mesh.delta_pos.w) + "; " + convert(mesh.delta_pos.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_rot = {" + convert(mesh.delta_rot.x) + "; " + convert(mesh.delta_rot.y) + "; " + convert(mesh.delta_rot.z) + "; " + convert(mesh.delta_rot.w) + "; " + convert(mesh.delta_rot.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_sca = {" + convert(mesh.delta_sca.x) + "; " + convert(mesh.delta_sca.y) + "; " + convert(mesh.delta_sca.z) + "; " + convert(mesh.delta_sca.w) + "; " + convert(mesh.delta_sca.axis_angle) + "}\n";
                outputStr += genIndent() + "delta_quat = {" + convert(mesh.delta_quat.rad) + "; " + convert(mesh.delta_quat.x) + "; " + convert(mesh.delta_quat.y) + "; " + convert(mesh.delta_quat.z) + "; " + convert(mesh.delta_quat.quatted) + "}\n";

                outputStr += genIndent() + "rotation_type = " + convert(mesh.rotation_type) + "\n";
		*/

                indent--;
                outputStr += genIndent() + "</Mesh>\n\n";

        }

	void outputVertex(FE_Vertex& vertex, size_t vertex_id) {
		outputStr += genIndent() + "<Vertex n=" + convert(vertex_id) + ">\n";
		indent++;

		outputStr += genIndent() + "x = " + convert(vertex.x) + "\n";
		outputStr += genIndent() + "y = " + convert(vertex.y) + "\n";
		outputStr += genIndent() + "z = " + convert(vertex.z) + "\n";

                outputStr += genIndent() + "normalx = " + convert(vertex.normalx) + "\n";
                outputStr += genIndent() + "normaly = " + convert(vertex.normaly) + "\n";
                outputStr += genIndent() + "normalz = " + convert(vertex.normalz) + "\n\n";
		
		outputStr += genIndent() + "id = " + convert(vertex.n) + "\n\n";
		
		for (auto& uvmap : vertex.uvmaps)
			outputUVMap(*uvmap);

		indent--;
		outputStr += genIndent() + "</Vertex>\n\n";
	}

	void outputUVMap(FE_UVMap& uvmap) {
		outputStr += genIndent() + "<UVMap name=\"" + uvmap.name + "\"\n";
		indent++;

		outputStr += genIndent() + "u = " + convert(uvmap.u) + "\n";
		outputStr += genIndent() + "v = " + convert(uvmap.v) + "\n";

		indent--;
		outputStr += genIndent() + "</UVMap>\n\n";
	}

	void outputVertexGroup(FE_VertexGroup& vgroup) {
		outputStr += genIndent() + "<VertexGroup name=\"" + vgroup.name + "\">\n";
		indent++;

		outputStr += genIndent() + "material = \"" + vgroup.material + "\"\n";
		outputStr += genIndent() + "bone_name = \"" + vgroup.bone_name + "\"\n";
		outputStr += genIndent() + "polygons = {";
		for (auto& polygon : vgroup.polygons) outputStr += "\"" + convert(polygon) + "\"; ";
		outputStr = outputStr.substr(0, outputStr.size()-2);
		outputStr += "}\n";

		indent--;
		outputStr += genIndent() + "</VertexGroup>\n\n";
	}

	void outputAnimation(FE_Animation& animation) {
                outputStr += genIndent() + "<Animation name=\"" + animation.name + "\">\n";
                indent++;

		outputStr += genIndent() + "frameNum = " + convert(animation.frameNum) + "\n";
		outputStr += genIndent() + "inverse_kinematics = " + convert(animation.inverse_kinematics) + "\n\n";
		outputStr += genIndent() + "name = " + convert(animation.name) + "\n\n";
		for (auto& transformation : animation.transforms)
			outputTransformation(*transformation);

                indent--;
                outputStr += genIndent() + "</Animation>\n\n";

	}

	void outputTransformation(FE_Transformation& transformation) {
		outputStr += genIndent() + "<Transformation bonename=\"" + transformation.bonename + "\">\n";
		indent++;

                outputStr += genIndent() + "pos = {" + convert(transformation.pos.x) + "; " + convert(transformation.pos.y) + "; " + convert(transformation.pos.z) + "; " + convert(transformation.pos.w) + "; " + convert(transformation.pos.axis_angle) + "}\n";
                outputStr += genIndent() + "rot = {" + convert(transformation.rot.x) + "; " + convert(transformation.rot.y) + "; " + convert(transformation.rot.z) + "; " + convert(transformation.rot.w) + "; " + convert(transformation.rot.axis_angle) + "}\n";
                outputStr += genIndent() + "sca = {" + convert(transformation.sca.x) + "; " + convert(transformation.sca.y) + "; " + convert(transformation.sca.z) + "; " + convert(transformation.sca.w) + "; " + convert(transformation.sca.axis_angle) + "}\n";

		indent--;
		outputStr += genIndent() + "</Transformation>\n";
	}

	void outputPolygon(FE_Polygon& polygon) {
                outputStr += genIndent() + "<Plygon n=" + convert(polygon.n) + ">\n";
                indent++;

		outputStr += genIndent() + "fake_normal = " + convert(polygon.fake_normal) + "\n";
		outputStr += genIndent() + "normal = {" + convert(polygon.normal.x) + "; " + convert(polygon.normal.y) + "; " + convert(polygon.normal.z) + "; " + convert(polygon.normal.w) + "; " + convert(polygon.normal.axis_angle) + "}\n";
		outputStr += genIndent() + "vertexNums = {";
		for (auto& num : polygon.vertexNums) outputStr += convert(num) + "; ";
		outputStr = outputStr.substr(0, outputStr.size()-2);
		outputStr += "}\n";

                indent--;
                outputStr += genIndent() + "</Polygon>\n\n";

	}

	void outputPhysics(FE_Physics *physics) {
                outputStr += genIndent() + "<Physics>\n";
                indent++;

		outputStr += genIndent() + "bounciness = \"" + physics->bounciness + "\"\n";
		outputStr += genIndent() + "vehicle = " + convert(physics->vehicle) + "\n";
		outputStr += genIndent() + "tire = " + convert(physics->tire) + "\n";

                indent--;
                outputStr += genIndent() + "</Physics>\n\n";

	}

        void outputTexture(FE_Texture& texture) {
                outputStr += genIndent() + "<Texture name=\"" + texture.name + "\">\n";
                indent++;

		outputStr += genIndent() + "path = \"" + texture.path + "\"\n";
		outputStr += genIndent() + "type = \"" + texture.type + "\"\n";
		outputStr += genIndent() + "stencil_name = \"" + texture.stencil_name + "\"\n";
		outputStr += genIndent() + "uv_map = \"" + texture.uv_map + "\"\n";
		outputStr += genIndent() + "stencil_num = " + convert(texture.stencil_num) + "\n";

                indent--;
                outputStr += genIndent() + "</Texture>\n\n";

        }

        void outputMaterial(FE_Material& material) {
                outputStr += genIndent() + "<Material name=\"" + material.name + "\">\n";
                indent++;

		outputStr += genIndent() + "shadeless = " + convert(material.shadeless) + "\n";
		outputStr += genIndent() + "dif = {" + convert(material.dif_r) + "; " + convert(material.dif_g) + "; " + convert(material.dif_b) + "; " + convert(material.dif_a) + "; " + "}\n";
		outputStr += genIndent() + "scol = {" + convert(material.scol_r) + "; " + convert(material.scol_g) + "; " + convert(material.scol_b) + "; " + convert(material.scol_a) + "; " + "}\n";
		outputStr += genIndent() + "alpha = " + convert(material.alpha) + "\n";
		outputStr += genIndent() + "specular_intensity = " + convert(material.specular_hardness) + "\n";
		outputStr += genIndent() + "specular_hardness = " + convert(material.specular_hardness) + "\n";
                outputStr += genIndent() + "translucency = {" + convert(material.translucency.x) + "; " + convert(material.translucency.y) + "; " + convert(material.translucency.z) + "; " + convert(material.translucency.w) + "; " + convert(material.translucency.axis_angle) + "; " + "}\n";
                outputStr += genIndent() + "illuminosity = {" + convert(material.illuminosity.x) + "; " + convert(material.illuminosity.y) + "; " + convert(material.illuminosity.z) + "; " + convert(material.illuminosity.w) + "; " + convert(material.illuminosity.axis_angle) + "; " + "}\n";
		outputStr += genIndent() + "cube_map = " + convert(material.cube_map) + "\n";
		outputStr += genIndent() + "cm_resolution = " + convert(material.cm_resolution) + "\n";
		outputStr += genIndent() + "texureNames = {";
		for (auto& name : material.textureNames) outputStr += name + "; ";
		outputStr = outputStr.substr(0, outputStr.size()-2);
		outputStr += "}\n";
		//vector<GLFloat> getMaterialData();

                indent--;
                outputStr += genIndent() + "</Material>\n\n";

        }

	void outputTCM(FE_TCM& tcm) {
                outputStr += genIndent() + "<TCM name=\"" + tcm.name + "\">\n";
                indent++;

		outputStr += genIndent() + "texureNames = {";
                for (auto& name : tcm.textureNames) outputStr += name + "; ";
                outputStr = outputStr.substr(0, outputStr.size()-2);
                outputStr += "}\n";

                outputStr += genIndent() + "translucency = {" + convert(tcm.value.x) + "; " + convert(tcm.value.y) + "; " + convert(tcm.value.z) + "; " + convert(tcm.value.w) + "; " + convert(tcm.value.axis_angle) + "; " + "}\n";

		outputStr += genIndent() + "tex_indices = {";
                for (auto& index : tcm.tex_indices) outputStr += index + "; ";
                outputStr = outputStr.substr(0, outputStr.size()-2);
                outputStr += "}\n";

                outputStr += genIndent() + "modes = {";
                for (auto& mode : tcm.modes) outputStr += convert(mode) + "; ";
                outputStr = outputStr.substr(0, outputStr.size()-2);
                outputStr += "}\n";

		outputStr += genIndent() + "combine_mode = \"" + tcm.combine_mode + "\"\n\n";

                indent--;
                outputStr += genIndent() + "</TCM>\n\n";

	}

	string genIndent() {
		string result = "";

		for (int i = 0; i < indent; i++)
			result += "\t";

		return result;
	}

	template <typename T>
	string convert(T item) {
		stringstream ss;
		ss << item;

		return ss.str();
	}

};

#endif
