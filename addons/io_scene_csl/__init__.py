

bl_info = {"name": "Export Oxygen Engine (.csl)",
		"author": "philsegeler@gmail.com",
			"category": "Import-Export",
			"location": "File > Import-Export",
			"blender": (2, 81, 6),
			"description": ("Export to Oxygen Engine CSL: meshes, uvs, materials, textures, "
					"armatures and animation"),
			#"warning": "Incomplete and subject to change",
			"version": (0,2,0)
			}

import bpy
from bpy_extras.io_utils import ExportHelper
#from bpy.props import *
import sys, os, math, mathutils

from bpy_extras.wm_utils.progress_report import (
    ProgressReport,
    ProgressReportSubstep,
)

print ([key for key in sys.modules.keys() if "FE" in key])
from .OE_Utils import CSL_Writer as f

# this is used to triangulate a mesh upon export

def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

# round floats

def round_floats(lista):
    return [f.convert(i) for i in lista]

# always returns a quaternion
def handleObjectRotation(obj):
    if "X" in obj.rotation_mode and "Y" in obj.rotation_mode and "Z" in obj.rotation_mode:
        return obj.rotation_euler.to_quaternion()
    elif obj.rotation_mode == "QUATERNION":
        return obj.rotation_quaternion
    elif obj.rotation_mode == "AXIS_ANGLE":
        return mathutils.Quaternion(mathutils.Vector(obj.rotation_axis_angle[1:4]), obj.rotation_axis_angle[0])
    else:
        return [2.0, 0.0, 0.0, 0.0]

#########################################
#handle all different blender objects
#TODO: Textures, TCMs, Nodes in Materials


# return an OE_Scene
def handle_scene(progress, scene, allowed_objects):
    cur_scene = f.OE_Scene(scene.name)
                
    materials = set()
                
    for obj in allowed_objects:
                    
        if type(obj.data) == bpy.types.Mesh:
            for mat in obj.data.materials:
                materials.add(mat)
                            
    for mat in materials:
        cur_scene.materials.append(handle_material(progress, cur_scene, mat))
                    
    for obj in allowed_objects:
        if type(obj.data) == bpy.types.Mesh:
            cur_scene.meshes.append(handle_mesh(progress, cur_scene, obj))
        elif type(obj.data) in [bpy.types.PointLight, bpy.types.SunLight, bpy.types.SpotLight]:
            cur_scene.lights.append(handle_light(progress, cur_scene, obj))
        elif type(obj.data) == bpy.types.Camera:
            cur_scene.cameras.append(handle_camera(progress, cur_scene, obj))
        else:
            pass
        
    return cur_scene

# return an OE_Light
def handle_light(progress, cur_scene, obj):
    light = f.OE_Light(obj.name)
    light.current_state.extend(list(obj.location))
    light.current_state.extend(list(handleObjectRotation(obj)))
    light.current_state.extend(list(obj.scale))
    
    if obj.parent != None:
        light.parent = obj.parent.name
        light.parent_type = 1
        
    light.color = list(obj.data.color)
    light.intensity = obj.data.energy
    light.range_ = obj.data.cutoff_distance 
    
    if type(obj.data) == bpy.types.PointLight:
        light.light_type = 1
    elif type(obj.data) == bpy.types.SunLight:
        light.light_type = 2
    elif type(obj.data) == bpy.types.SpotLight:
        light.light_type = 3
        light.fov = obj.data.spot_size
    return light

# return an OE_Mesh
def handle_mesh(progress, cur_scene, obj):
    
    mesh = f.OE_Mesh(obj.name)
    mesh.current_state.extend(list(obj.location))
    mesh.current_state.extend(list(handleObjectRotation(obj)))
    mesh.current_state.extend(list(obj.scale))
    
    if obj.parent != None:
        mesh.parent = obj.parent.name
        mesh.parent_type = 1
    
    # copy the original mesh and triangulate
    
    temp_mesh = obj.to_mesh()
    mesh_triangulate(temp_mesh)
    temp_mesh.calc_normals_split()
    
    # create all vertex groups (materials only for now, but will add armatures in the future)
    for material in obj.data.materials:
        material_vertex_group = f.OE_VertexGroup(material.name)
        material_vertex_group.material_id = material.name
        mesh.triangle_groups.append(material_vertex_group)
    
    # export vertices
    for v in temp_mesh.vertices[:]:
        mesh.vertices.extend( v.co[:])
    mesh.num_of_triangles = len(temp_mesh.polygons)
    
    # export uvmaps
    for uv_map in temp_mesh.uv_layers:
        uvmap = f.OE_UVMapData(uv_map.name, [])
        mesh.uvmaps.append(uvmap) 
    
    # export normals and polygons
    
    # these normal and uv dictionaries make the code muuuuuuch faster
    # they count the times a particular normal/uv has come up
    normal_dict = {}
    uv_dicts = [{} for i in temp_mesh.uv_layers]
    
    smooth_normals = False;
    # decide if smooth or not mesh
    for tri in temp_mesh.polygons:
        if tri.use_smooth:
            smooth_normals = True
            break
    
    if smooth_normals:
        mesh.normals = [0.0 for i in range(len(mesh.vertices))]
    else:
        mesh.normals = [0.0 for i in range(len(temp_mesh.polygons)*3)]
    
    tri_count = 0
    for tri in temp_mesh.polygons:
        
        poly = f.OE_Triangle()
        #print("triangle ;", tri_count)
        vertex_count=0
        for li_id in tri.loop_indices:
            vertex_count += 1
            li = temp_mesh.loops[li_id]
            
            # determine which vertex to assess
            cur_v = None
            if vertex_count == 1:
                cur_v = poly.v1
            elif vertex_count == 2:
                cur_v = poly.v2
            elif vertex_count == 3:
                cur_v = poly.v3
            else:
                print ("Is this a quad or polygon? This is NOT supposed to happen. ")
            
            # store vertex position index
            cur_v.append(li.vertex_index)
            
            
            # store vertex normal index 
            # and update mesh.normals
            nor = list(li.normal)
            if smooth_normals:
                cur_v.append(li.vertex_index)
                mesh.normals[li.vertex_index*3]   = nor[0]
                mesh.normals[li.vertex_index*3+1] = nor[1]
                mesh.normals[li.vertex_index*3+2] = nor[2]
            else:
                cur_v.append(tri_count)
                mesh.normals[tri_count*3]   = nor[0]
                mesh.normals[tri_count*3+1] = nor[1]
                mesh.normals[tri_count*3+2] = nor[2]
            
            
            #nor = list(li.normal)
            #nor_simplified = round_floats(nor)
            #if normal_dict.get(tuple(nor_simplified)) != None:
            #    cur_v.append(normal_dict[tuple(nor_simplified)])
            #else:
            #    cur_v.append(len(mesh.normals)//3)
            #    normal_dict[tuple(nor_simplified)] = len(mesh.normals)//3
            #    mesh.normals.extend(nor)
                
            # store uv map data
            # and update mesh.uvmaps
            uv_count = 0
            for uv_map in temp_mesh.uv_layers:
                uvmap = mesh.uvmaps[uv_count]
                
                uv_id = tri_count * 3 + vertex_count - 1
                actual_uv = list(uv_map.data[uv_id].uv)
                actual_uv_simplified = round_floats(actual_uv)
                if uv_dicts[uv_count].get(tuple(actual_uv_simplified)) != None:
                    cur_v.append(uv_dicts[uv_count][tuple(actual_uv_simplified)])
                else:
                    cur_v.append(len(uvmap.elements)//2)
                    uv_dicts[uv_count][tuple(actual_uv_simplified)] = len(uvmap.elements)//2
                    uvmap.elements.extend(actual_uv[:])
                uv_count += 1
            
        # append to vertex groups
        for vg in range(len(mesh.triangle_groups)):
            if mesh.triangle_groups[vg].material_id == obj.data.materials[tri.material_index].name:
                mesh.triangle_groups[vg].polygons.append(tri.index)
            
        mesh.triangles.append(poly)
        tri_count += 1
    mesh.num_of_triangles = len(mesh.triangles)
    return mesh

# return an OE_Camera
def handle_camera(progress, cur_scene, obj):
    camera = f.OE_Camera(obj.name)
    camera.current_state.extend(list(obj.location))
    camera.current_state.extend(list(handleObjectRotation(obj)))
    camera.current_state.extend(list(obj.scale))
    
    if obj.parent != None:
        camera.parent = obj.parent.name
        camera.parent_type = 1
        
    camera.aspect_ratio = 16.0/9.0
    camera.near = obj.data.clip_start
    camera.far = obj.data.clip_end
    camera.fov = obj.data.angle
    
    
    return camera

# return an OE_Material
def handle_material(progress, cur_scene, material):
    mat = f.OE_Material(material.name)
    
    nodes = material.node_tree.nodes
    
    for node in nodes:
        if "Base Color" in node.inputs:
            mat.dif_r = node.inputs['Base Color'].default_value[0]
            mat.dif_g = node.inputs['Base Color'].default_value[1]
            mat.dif_b = node.inputs['Base Color'].default_value[2]
            
        elif "Color" in node.inputs:
            mat.dif_r = node.inputs['Color'].default_value[0]
            mat.dif_g = node.inputs['Color'].default_value[1]
            mat.dif_b = node.inputs['Color'].default_value[2]
        else:
            pass
        
        if node.name == "Specular":            
            mat.scol_r = node.inputs['Specular'].default_value[0]
            mat.scol_g = node.inputs['Specular'].default_value[1]
            mat.scol_b = node.inputs['Specular'].default_value[2]
        
        if "Roughness" in node.inputs:    
            mat.specular_hardness = node.inputs['Roughness'].default_value
        
        mat.specular_intensity = 1.0
            
        mat.alpha = 1.0
    
    return mat

def writeToFile(fdir, data):
    filea = open(fdir, "w")
    filea.write(data)
    filea.close()


################################################
# Main exporting and menu classes

class ExportOxygenEngine(bpy.types.Operator, ExportHelper):
    bl_idname = "export_scene.csl"  
    bl_label = "Export CSL"
    bl_options = {'PRESET'}
    check_extension = True
    
    # export_helper
    filename_ext = ".csl"

    filter_glob : bpy.props.StringProperty( default="*.csl", options={'HIDDEN'},
        )
    
    use_selection: bpy.props.BoolProperty(
            name="Only selected objects",
            description="Export only selected objects (and associated materials and textures)",
            default=False,
            )
    
    def draw(self, context):
        pass
    
    def execute(self, context):
        
        cur_world = f.OE_World()
        
        # get out of edit mode
        if bpy.ops.object.mode_set.poll():
            bpy.ops.object.mode_set(mode='OBJECT')
        
        progress = ""
        # handle only selected objects if use_selection is true
        # only add scene information
        if self.use_selection:
            final_scene = handle_scene(progress, context.scene, context.selected_objects)
            cur_world.scenes.append(final_scene)
        
            # handle all objects in all scenes
        else:
            for scene in bpy.data.scenes:
                cur_world.scenes.append(handle_scene(progress, scene, scene.objects))
        
        # add viewport config
        # if a camera is the active object, then it goes to the viewport
        # else the exporter just uses the first camera it finds
        # It is the exact same default as the default viewport config generated in C++
        vp_config = f.OE_ViewportConfig("default")
        vp_config.layer_combine_modes.append(1)
        vp_config.split_screen_positions.append(0.5)
        vp_config.split_screen_positions.append(0.5)
        
        vp_config.camera_modes.append(1)
        vp_config.layers.append(0)
        camera_name = ""
        
        if type(context.active_object.data) == bpy.types.Camera:
            camera_name = context.active_object.name
            
        else:
            foundCam = False
            for scene in cur_world.scenes:
                if foundCam: break
                
                for camera in scene.cameras:
                    camera_name = camera.name
                    foundCam = True
                    break
        
        vp_config.cameras.append(camera_name)
            
        cur_world.viewports.append(vp_config)
        cur_world.loaded_viewport = "default"
        writeToFile(self.filepath, str(cur_world))
        
        return {"FINISHED"}



class CSL_PT_export_include(bpy.types.Panel):
    bl_space_type = 'FILE_BROWSER'
    bl_region_type = 'TOOL_PROPS'
    bl_label = "Include"
    bl_parent_id = "FILE_PT_operator"

    @classmethod
    def poll(cls, context):
        sfile = context.space_data
        operator = sfile.active_operator

        return operator.bl_idname == "EXPORT_SCENE_OT_csl"

    def draw(self, context):
        layout = self.layout
        #layout.use_property_split = True
        layout.use_property_decorate = False
        
        sfile = context.space_data
        operator = sfile.active_operator
        
        layout.prop(operator, 'use_selection')
        
        

classes = tuple([ExportOxygenEngine, CSL_PT_export_include])

def menu_func_export(self, context):
	self.layout.operator(ExportOxygenEngine.bl_idname, text="Oxygen Engine (.csl)")

def register():
    for cls in classes:
        bpy.utils.register_class(cls)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)
    
def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()
