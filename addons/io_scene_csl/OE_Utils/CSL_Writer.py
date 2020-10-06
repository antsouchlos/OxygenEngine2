#--------------------------CSL_WRITER-BASE-------------------------------#
##########################################################################
# ATTENTION: HUGE REWRITE IN PROGRESS !!!!!
# 2020/10/02 Status: Initial updated version done
# for any new variable use the REALLY USEFUL functions below for optimal performance

## REALLY USEFUL FUNCTIONS (not intended to be readable, but efficient)

#used to convert lists to needed .csl format
	
#used to write non-list variables.. in 1 freaking line
def outputVar(indent, name, var, suffix=""):
	return "".join([indent, name, "=", convert(var), suffix]) #only use first 16 digits in floats
		
#used to write lists to the .fle format or not write them at all because im bored... in 1 freaking line
def outputList(indent, name, actual_list):
	return ("".join([indent, name, " = { ", " ; ".join([convert(i) for i in actual_list]), " }"]) if len(actual_list) else "")

#used to write types.. because im bored... in 1 freaking line
#NOTE: args is a dictionary in the form {varname0: value0, ..., varnameX: valueX}
def outputTypeTag(indent, name, args):
    return "".join( [indent, "".join(["<", name, str("" if len(args) == 0 else " " )]), " ".join([outputVar("", key, args[key], "") for key in args.keys()]), ">" ] )

def outputClosingTag(indent, name):
	return "".join([indent, "</", name, ">"])

#used to remove excess '\n's from the output
def optimizeList(a_list):
    while "" in a_list:
        a_list.remove("")
    return a_list

#----------------------------TYPES-------------------------------#

#defines the floating point rounding factor
OE_float_rounding_factor = 8

#used to convert variables to needed .fle format (including floats and strings)
def convert(var):
	if not type(var) is float:
		return (''.join([''.join(['"', var, '"']) if type(var) == str else str(var)]) )
	elif not type(var) is bool:
		return "".join(["{0:.",str(OE_float_rounding_factor), "f}"]).format(var)
	else: return str(int(var))

######################

class OE_WriterBase:
    indent = 0
    @staticmethod
    def genIndent():
        return "".join(["\t" for i in range(0, OE_WriterBase.indent)])
    
    def __init__(self):
        self.classname = "OE_WriterBase"

#######################

class OE_UVMapData(OE_WriterBase):
    def __init__(self, name_="", elements_2=[]):
        self.classname = "UVMapData"
        
        self.name = name_
        self.elements = elements_2
        self.num_of_uvs = 0
    
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name})
        OE_WriterBase.indent += 1
        
        self.num_of_uvs = len(self.elements)
        
        temp1 = outputVar(OE_WriterBase.genIndent(), "num_of_uvs", self.num_of_uvs)
        temp2 = outputList(OE_WriterBase.genIndent(), "elements", self.elements)
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, temp2, tempn]))

########################

class OE_Triangle(OE_WriterBase):
    def __init__(self):
        self.classname = "Triangle"
        
        self.v1 = []
        self.v2 = []
        self.v3 = []
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, dict())
        OE_WriterBase.indent += 1
        
        temp1 = outputList(OE_WriterBase.genIndent(), "v1", self.v1)
        temp2 = outputList(OE_WriterBase.genIndent(), "v2", self.v2)
        temp3 = outputList(OE_WriterBase.genIndent(), "v3", self.v3)
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, temp2, temp3, tempn]))

########################

class OE_VertexGroup(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "VertexGroup"
        
        self.name = name_
        
        self.polygons    = [] #list of integers
        self.material_id = ""
        self.bone_id     = ""
        self.visible     = True

    def __str__(self):
         
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name":self.name, "visible":int(self.visible)})
        OE_WriterBase.indent += 1
        
        temp1 = outputList(OE_WriterBase.genIndent(), "polygons", self.polygons)
        
        temp2 = outputVar(OE_WriterBase.genIndent(), "material_id", self.material_id)
        temp3 = outputVar(OE_WriterBase.genIndent(), "bone_id", self.bone_id)
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp2, temp3, temp1, tempn]))

########################

class OE_Mesh(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "Mesh"
        
        self.name            = name_
        self.vertices        = [] #list of floats
        self.normals         = [] #list of floats
        self.uvmaps          = [] #list of OE_UVMapDatas
        
        self.isDynamic       = False
        self.visible         = True
        self.triangles       = [] #list of OE_Triangles
        self.triangle_groups = [] #list of OE_TriangleGroups
        
        self.current_state   = [] #list of 10 floats
        self.textureCM_IDs   = [] #list of strings
        
        self.parent          = "" 
        self.parent_type     = 0 
        self.num_of_triangles= 0
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name, "isDynamic":int(self.isDynamic), "visible":int(self.visible), "num_uvs":len(self.uvmaps)})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputList(OE_WriterBase.genIndent(), "current_state", self.current_state),
                            outputVar(OE_WriterBase.genIndent(), "parent", self.parent),  
                            outputVar(OE_WriterBase.genIndent(), "parent_type", self.parent_type),
                            outputVar(OE_WriterBase.genIndent(), "num_of_vertices", len(self.vertices)),
                            outputVar(OE_WriterBase.genIndent(), "num_of_normals", len(self.normals)),
                            outputVar(OE_WriterBase.genIndent(), "num_of_triangles", self.num_of_triangles),
                            outputList(OE_WriterBase.genIndent(), "vertices", self.vertices),
                            outputList(OE_WriterBase.genIndent(), "normals", self.normals),
                            outputList(OE_WriterBase.genIndent(), "textureCM_IDs", self.textureCM_IDs)]))
        
        temp2 = "\n".join(optimizeList([ "\n".join([ str(k) for k in self.uvmaps]),
                            "\n".join([ str(k) for k in self.triangles]),
                            "\n".join([ str(k) for k in self.triangle_groups])]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, temp2, tempn]))

#########################

class OE_Light(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "Light"
        
        self.name            = name_
        self.parent          = "" 
        self.parent_type     = 0 
        self.current_state   = [] #list of 10 floats
        self.visible         = True
        
        self.light_type  = 0
        self.objects     = [] #list of strings
        self.color       = [] #list of 3 floats
        self.intensity   = 0.0
        self.fov         = 0.0
        self.range_      = 0.0
    
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name, "visible":int(self.visible)})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputList(OE_WriterBase.genIndent(), "current_state", self.current_state),
                            outputVar(OE_WriterBase.genIndent(), "parent", self.parent),  
                            outputVar(OE_WriterBase.genIndent(), "parent_type", self.parent_type),
                            outputList(OE_WriterBase.genIndent(), "objects", self.objects),
                            outputList(OE_WriterBase.genIndent(), "color", self.color),
                            outputVar(OE_WriterBase.genIndent(), "intensity", self.intensity),
                            outputVar(OE_WriterBase.genIndent(), "fov", self.fov),
                            outputVar(OE_WriterBase.genIndent(), "light_type", self.light_type),
                            outputVar(OE_WriterBase.genIndent(), "range", self.range_)]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_Camera(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "Camera"
        
        self.name            = name_
        self.parent          = "" 
        self.parent_type     = 0 
        self.current_state   = [] #list of 10 floats
        self.visible         = True
        
        self.aspect_ratio    = 0.0
        self.fov             = 0.0
        self.near            = 0.0
        self.far             = 0.0
        
    
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name, "visible":int(self.visible)})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputList(OE_WriterBase.genIndent(), "current_state", self.current_state),
                            outputVar(OE_WriterBase.genIndent(), "parent", self.parent),  
                            outputVar(OE_WriterBase.genIndent(), "parent_type", self.parent_type),
                            outputVar(OE_WriterBase.genIndent(), "fov", self.fov),
                            outputVar(OE_WriterBase.genIndent(), "aspect_ratio", self.aspect_ratio),
                            outputVar(OE_WriterBase.genIndent(), "far", self.far),
                            outputVar(OE_WriterBase.genIndent(), "near", self.near)]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_Material(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "Material"
        
        self.name               = name_
        self.dif_r              = 0.0
        self.dif_g              = 0.0
        self.dif_b              = 0.0
        self.dif_a              = 1.0
        self.scol_r             = 0.0
        self.scol_g             = 0.0
        self.scol_b             = 0.0
        self.alpha              = 1.0
        self.translucency       = 0.0
        self.illuminosity       = 0.0
        self.specular_intensity = 0.0
        self.specular_hardness  = 0.0
        self.textureCM_IDs      = [] #list of strings
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputList(OE_WriterBase.genIndent(), "textureCM_IDs", self.textureCM_IDs),
                            outputVar(OE_WriterBase.genIndent(), "dif_r", self.dif_r),
                            outputVar(OE_WriterBase.genIndent(), "dif_g", self.dif_g),  
                            outputVar(OE_WriterBase.genIndent(), "dif_b", self.dif_b),  
                            outputVar(OE_WriterBase.genIndent(), "dif_a", self.dif_a),
                            outputVar(OE_WriterBase.genIndent(), "scol_r", self.scol_r), 
                            outputVar(OE_WriterBase.genIndent(), "scol_g", self.scol_g), 
                            outputVar(OE_WriterBase.genIndent(), "scol_b", self.scol_b),  
                            outputVar(OE_WriterBase.genIndent(), "alpha", self.alpha), 
                            outputVar(OE_WriterBase.genIndent(), "translucency", self.translucency),
                            outputVar(OE_WriterBase.genIndent(), "illuminosity", self.illuminosity),
                            outputVar(OE_WriterBase.genIndent(), "specular_hardness", self.specular_hardness),
                            outputVar(OE_WriterBase.genIndent(), "specular_intensity", self.specular_intensity)]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_Texture(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "Texture"
        
        self.name               = name_
        self.source             = 0
        self.camera             = ""
        self.path               = ""
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputVar(OE_WriterBase.genIndent(), "source", self.source),
                            outputVar(OE_WriterBase.genIndent(), "path", self.path),  
                            outputVar(OE_WriterBase.genIndent(), "camera", self.camera)]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_TCM_Texture(OE_WriterBase):
    def __init__(self,):
        self.classname = "TCM_Texture"
        
        self.textureID          = ""
        self.mode               = 0 # (0=undefined 1=add 2=subtract 3=mix 4=stencil, 5=custom shader)
        self.textureMulFactor   = 1
        self.uvmap              = 1
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, dict())
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputVar(OE_WriterBase.genIndent(), "textureID", self.textureID),  
                            outputVar(OE_WriterBase.genIndent(), "mode", self.mode),  
                            outputVar(OE_WriterBase.genIndent(), "textureMulFactor", self.textureMulFactor),  
                            outputVar(OE_WriterBase.genIndent(), "uvmap", self.uvmap)]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_TCM(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "TextureCombineMode"
        
        self.name               = name_
        self.combine_mode       = 0 # 0=undefined 1=DIFFUSE 2=SPECULAR 3=NORMAL 4=HEIGHT 5=GLOW
        self.texture_array      = False
        self.r                  = 0.0
        self.g                  = 0.0
        self.b                  = 0.0
        self.a                  = 0.0
        self.textures           = [] # list of OE_TCM_Textures
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputVar(OE_WriterBase.genIndent(), "texture_array", int(self.texture_array)),
                            outputVar(OE_WriterBase.genIndent(), "r", self.r),  
                            outputVar(OE_WriterBase.genIndent(), "g", self.g),  
                            outputVar(OE_WriterBase.genIndent(), "b", self.b),  
                            outputVar(OE_WriterBase.genIndent(), "a", self.a),  
                            outputVar(OE_WriterBase.genIndent(), "combine_mode", self.combine_mode)]))
        
        temp2 = "\n".join(optimizeList([ str(k) for k in self.textures]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_Scene(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "Scene"
        
        self.name               = name_
        self.textures           = [] # list of OE_Textures
        self.texture_CMs        = [] # list of OE_TCMs
        self.materials          = [] # list of OE_Materials
        self.cameras            = [] # list of OE_Cameras
        self.meshes             = [] # list of OE_Meshes
        self.lights             = [] # list of OE_Lights
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ "\n".join([ str(k) for k in self.textures]),
                            "\n".join([ str(k) for k in self.texture_CMs]),
                            "\n".join([ str(k) for k in self.materials]),
                            "\n".join([ str(k) for k in self.meshes]),
                            "\n".join([ str(k) for k in self.lights]),
                            "\n".join([ str(k) for k in self.cameras])]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))
    
#########################

class OE_ViewportConfig(OE_WriterBase):
    def __init__(self, name_=""):
        self.classname = "ViewportConfig"
        
        self.name               = name_
        self.layers             = [] # list of integers
        self.cameras            = [] # list of strings
        self.camera_modes       = [] # list of integers
        self.layer_combine_modes    = [] # list of integers
        self.split_screen_positions = [] # list of floats 

    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, {"name": self.name})
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputList(OE_WriterBase.genIndent(), "layers", self.layers),
                            outputList(OE_WriterBase.genIndent(), "cameras", self.cameras),
                            outputList(OE_WriterBase.genIndent(), "camera_modes", self.camera_modes),
                            outputList(OE_WriterBase.genIndent(), "layer_combine_modes", self.layer_combine_modes),
                            outputList(OE_WriterBase.genIndent(), "split_screen_positions", self.split_screen_positions)]))
                            
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp1, tempn]))

#########################

class OE_World(OE_WriterBase):
    def __init__(self):
        self.classname = "World"
        
        self.loaded_scene       = ""
        self.loaded_viewport    = ""
        self.scenes             = [] # list of OE_Scenes
        self.viewports          = [] # list of OE_ViewportConfig
        
    def __str__(self):
        temp0 = outputTypeTag(OE_WriterBase.genIndent(), self.classname, dict())
        OE_WriterBase.indent += 1
        
        temp1 = "\n".join(optimizeList([ outputVar(OE_WriterBase.genIndent(), "loaded_scene", self.loaded_scene), 
                            outputVar(OE_WriterBase.genIndent(), "loaded_viewport", self.loaded_viewport)]))
        
        temp2 = "\n".join(optimizeList([ "\n".join([ str(k) for k in self.scenes]),
                            "\n".join([ str(k) for k in self.viewports])]))
        
        OE_WriterBase.indent -= 1
        tempn = outputClosingTag(OE_WriterBase.genIndent(), self.classname)
        return "\n".join(optimizeList([temp0, temp2, temp1, tempn]))


mesh = OE_Mesh("kapoios")

scene = OE_Scene("malakeia")
scene.meshes.append(mesh)

scene2 = OE_Scene("deyteri")

cam = OE_World()
cam.scenes.extend([scene, scene2])
stra = str(cam)
print(stra)


