class FE_Vec4:
    def __init__(x=0.0, y=0.0, z=0.0, w=0.0):
	self.x = x
	self.y = y
	self.z = z
	self.w = w
    def __str__(self):
        return "</" + str(self.x) + "/" + str(self.y) + "/" + str(self.z) + "/>"
class UVMap:
    def __init__(self):
        self.name = ""
        self.u = 0.0
        self.v = 0.0
    def log():
        return "UVmap{\n"+ "  name: " + name + " u: " + str(u) + " v: " + str(v) + "\n}\n"

class Vertex:
    def __init__(self):
        self.n = 0

        self.x = 0.0
        self.y = 0.0
        self.z = 0.0

        self.normal_x = 0.0
        self.normal_y = 0.0
        self.normal_z = 0.0

        self.uvmaps = []
    def log():
        return "Vertex{\n" + "  index: " + str(n) + "\n " + str(x) + " " + str(normal_x) + "\n " + str(y)+ " " + str(normal_y) + "\n "+ str(z) + " " + str(normal_z) + "\n " + str([uv.log() for uv in uvmaps]) + "\n}\n" 

class Polygon:
    def __init__(self):
        self.n = 0
        self.normal = 0

        self.vertexNums = []
    def log():
        return "Polygon{\n" + " index: " + str(n) + "\n " + sum([str(v) + " " for v in vertexNums]) + "\n}\n"

class VertexGroup:
    def __init__(self):
        self.name = ""
        self.material = ""
        self.bone_name = ""

        self.polygons = []
    def log():
        return "VertexGroup{\n" + " name: " + name + "\n material: " + material + "\n bone: " + bone_name + "\n " + sum([str(v) for v in polygons]) +"\n}\n"

class Transformation:
    bonname = ""

    pos = FE_Vec4()
    rot = FE_Vec4()
    sca = FE_Vec4()

class Animation:
    frameNum = 0

    inverse_kinematics = False

    transforms = []

class Texture:
    def __init__(self):
        self.name = ""
        self.path = ""

        self.type = ""
        self.stencil_name = ""

        self.uv_map = ""
        self.stencil_num = 0
    def log():
        return sum("Texture{\n", " name: ", name, "\n path: ", path, "\n typea: ", typea, "\n stencil_name: ", stencil_name, "\n uv_map: ", uv_map, "\n stencil_num: ", stencil_num, "\n}\n")
class Material:
    def __init__(self):
        self.name = ""
        self.shadeless = False

        self.dif_r = 0
        self.dif_g = 0
        self.dif_b = 0
        self.dif_a = 0

        self.scol_r = 0
        self.scol_g = 0
        self.scol_b = 0
        self.scol_a = 0

        self.alpha = 0.0

        self.specular_intensity = 0.0
        self.specular_hardness = 0.0

        self.translucency = FE_Vec4();
        self.textureNames = []

class TextureCombineMode:
    def __init__(self):
        self.name = ""
        self.combine_mode = ""
        self.value        = FE_Vec4()
        # new code
        self.textures     = [] #string array
        self.tex_indices  = [] #int array
        self.modes    = [] #string array
        ####
        self.textureNames = []

class Interaction:
    objectNames = []

    interaction = ""

class Physics:
    bounciness = ""

    vehicle = False
    tire = False

class Object:
    def __init__(self):
        self.name = ""

        self.pos = FE_Vec4()
        self.rot = FE_Vec4()
        self.sca = FE_Vec4()

        self.vertices = [];
        self.vertex_groups = [];
        self.animations = [];
        self.physics = Physics();
        # new code
        self.polygons = []
    ####

class Camera:
    def __init__(self):
        self.name = ""

        self.pos = FE_Vec4()
        self.rot = FE_Vec4()

        self.aspect_ratio = 0.0
        self.field_of_view = 0.0

        self.near = 0
        self.far = 0

class Light:
    def __init__(a):
     a.name = ""

     a.pos = FE_Vec4()
     a.rot = FE_Vec4()

     a.light_type = ""

class Scene:
    def __init__(self):
        self.name = ""

        self.gravity = 0.0
        self.gravityAxis = ""

        self.groups = [];

        self.cameras = [];
        self.lights = [];

        self.interactions = [];
        self.objects = [];
        self.textures = [];
        self.materials = [];
        self.textureCombineModes = [];

class World:
    def __init__(self):
	 self.name = ""

         self.loaded_scene = ""

         self.scenes = [];
