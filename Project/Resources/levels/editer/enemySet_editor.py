import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json

bl_info = {
	"name": "EnemySetEditor",
    "author": "Sho Shimizu",
    "version": (1, 0),
    "blender": (3, 1, 2),
    "location": "3DVIEW > Sidebar > ToolTab",
    "description": "敵の名前を設定して生成するエディタ",
    "warning": "",
    "support": "COMMUNITY",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Object"
}

class ADDONAME_OT_EnemyOperator(bpy.types.Operator):
    bl_label = "追加"
    bl_idname = "wm.template_operator"
    bl_options = {"REGISTER", "UNDO"}

    min=0.0
    max=0.0
    
    #プロパティを設定
    preset_enum : bpy.props.EnumProperty(
        name="敵を選択",
        description= "Select Enemy",
        #属性分け
        items=[
            ('OP1', "NormalEnemy", "通常の敵を追加"),
            ('OP2', "BossEnemy", "ボスを追加"),
            ('OP3', "MoveEnemy", "動く敵を追加")
        ]
    )

    val_x : bpy.props.FloatProperty(name="X軸")
    val_y : bpy.props.FloatProperty(name="Y軸")
    val_z : bpy.props.FloatProperty(name="Z軸")
    
    def invoke(self, context, event):
        wm = context.window_manager
        return wm.invoke_props_dialog(self)
    
    def draw(self, context):
        layout = self.layout
        #敵を追加
        layout.prop(self, "preset_enum")

        layout.prop(self,"val_x")
        layout.prop(self,"val_y")
        layout.prop(self,"val_z")
    
    def execute(self, context):
        if self.preset_enum == 'OP1':      
            #正方形を追加
            bpy.ops.mesh.primitive_cube_add()

            #['file_name']カスタムプロパティを追加
            context.object["file_name"] = "normal"            

        if self.preset_enum == 'OP2':
            #ICO球を追加
            bpy.ops.mesh.primitive_ico_sphere_add()

            #['file_name']カスタムプロパティを追加
            context.object["file_name"] = "boss"

        if self.preset_enum == 'OP3':
            bpy.ops.mesh.primitive_uv_sphere_add(radius=0.5)
            planet = context.object
            bpy.ops.curve.primitive_bezier_curve_add()
            orbit = context.object
            
            fp = planet.constraints.new('FOLLOW_PATH')
            fp.target = orbit
            fp.use_fixed_location = True
            fp.use_curve_follow = True
            fcurve = fp.driver_add("offset_factor")
            fcurve.driver.expression = "(frame / %d) %% 1" % 50

            #['file_name']カスタムプロパティを追加
            context.object["file_name"] = "move"

        objects = [object for object in context.selected_objects]
        for object in objects:
            object.location.x += self.val_x
            object.location.y += self.val_y
            object.location.z += self.val_z

        return {'FINISHED'}
    
class ADDONNAME_PT_EnemyPanel(bpy.types.Panel):
    bl_label = "敵を選択"
    bl_idname = "ADDONNAME_PT_EnemyPanel"
    bl_space_type = "VIEW_3D"
    bl_region_type = 'UI'
    bl_category = "Add Enemy"
    
    def draw(self, context):
        layout = self.layout    
        layout.operator("wm.template_operator")

#Blenderに登録するクラスリスト
classes = {
       ADDONNAME_PT_EnemyPanel,
       ADDONAME_OT_EnemyOperator,
}

#アドオン有効時コールバック
def register():
    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.VIEW3D_MT_object.append(ADDONNAME_PT_EnemyPanel)
    bpy.types.VIEW3D_MT_object.append(ADDONAME_OT_EnemyOperator)
    #メニューに項目を追加
    print("レベルエディタが有効化されました。")

#アドオン無効時コールバック
def unregister():
    bpy.types.VIEW3D_MT_object.remove(ADDONNAME_PT_EnemyPanel)
    bpy.types.VIEW3D_MT_object.remove(ADDONAME_OT_EnemyOperator)
    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効時されました。")