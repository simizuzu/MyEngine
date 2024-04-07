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
   
class ADDONNAME_PT_EnemyPanel(bpy.types.Panel):
    bl_label = "Add Enemy"
    bl_idname = "ADDONNAME_PT_EnemyPanel"
    bl_space_type = "VIEW_3D"
    bl_region_type = 'UI'
    bl_category = "Add Enemy"
    
    def draw(self, context):
        layout = self.layout    
        layout.operator("wm.template_operator")
        
 
class ADDONAME_OT_EnemyOperator(bpy.types.Operator):
    bl_label = "敵を選択"
    bl_idname = "wm.template_operator"
    bl_options = {"REGISTER", "UNDO"}
    
    preset_enum : bpy.props.EnumProperty(
        name="敵を選択",
        description= "Select Enemy",
        items=[
            ('OP1', "NormalEnemy", "通常の敵を追加"),
            ('OP2', "BossEnemy", "ボスを追加"),
        ]
    )
    
    def invoke(self, context, event):
        wm = context.window_manager
        return wm.invoke_props_dialog(self)
    
    def draw(self, context):
        layout = self.layout
        layout.prop(self, "preset_enum")
    
    def execute(self, context):
        if self.preset_enum == 'OP1':
            bpy.ops.mesh.primitive_cube_add()
        
        if self.preset_enum == 'OP2':
            bpy.ops.mesh.primitive_ico_sphere_add()

        return {'FINISHED'}

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