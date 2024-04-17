import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json
import os

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

#通常の関数としてファイルの拡張子を検出してファイル形式を返す
def detect_file_format(file_path):
    file_extension = os.path.splitext(file_path)[1].lower()
    if file_extension == '.obj':
        return 'OBJ'
    elif file_extension == '.fbx':
        return 'FBX'
    else:
        return 'Unknown'

class ADDONAME_OT_EnemyOperator(bpy.types.Operator):
    bl_label = "追加"
    bl_idname = "wm.template_operator"
    bl_options = {"REGISTER", "UNDO"}

    #プロパティを設定
    preset_enum : bpy.props.EnumProperty(
        name="敵を選択",
        description= "敵を選択して生成するプロパティ",
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

    bpy.types.Scene.filepath = bpy.props.StringProperty(
        name= "ファイルパス",
        description= "モデルのファイルパスを指定して生成するプロパティ",
        default= "",
        maxlen=1024, #最大文字数
        subtype="FILE_PATH",
        options={'SKIP_SAVE'}
    )
    
    def invoke(self, context, event):
        wm = context.window_manager
        return wm.invoke_props_dialog(self, width=400)# ダイアログの幅を調整するために width を設定

    def draw(self, context):
        layout = self.layout
        scene = context.scene
        #敵を追加
        layout.prop(self, "preset_enum")

        #X,Y,Z座標を追加
        layout.prop(self,"val_x")
        layout.prop(self,"val_y")
        layout.prop(self,"val_z")

        # ファイルパスを入力するテキストボックスを表示
        layout.prop(scene, "filepath", text="")
    
    def execute(self, context):
        filepath = context.scene.filepath

        #ファイルパスが空だった時何も描画しない
        if filepath == '':
            return {'FINISHED'}
        else:
            #ファイル形式を検出する
            file_format = detect_file_format(filepath)

            #ファイル形式に応じてインポート操作を行う
            if file_format == "OBJ":
                bpy.ops.import_scene.obj(filepath=filepath)
            elif file_format == "FBX":
                bpy.ops.import_scene.fbx(filepath=filepath)

        #オペレーター01
        if self.preset_enum == 'OP1':  
            #インポートされたオブジェクトを取得する
            obj=bpy.context.selected_objects[0]

            #['file_name']カスタムプロパティを追加
            obj["file_name"] = "normal"
        
        #オペレーター02
        if self.preset_enum == 'OP2':
            #インポートされたオブジェクトを取得する
            obj=bpy.context.selected_objects[0]

            #['file_name']カスタムプロパティを追加
            obj["file_name"] = "boss"

        #オペレーター03
        if self.preset_enum == 'OP3':
            #インポートされたオブジェクトを取得する
            obj = bpy.context.selected_objects[0]
            #カーブを生成
            bpy.ops.curve.primitive_bezier_curve_add()
            # 生成されたベジェ曲線オブジェクトを取得する
            curve = context.object

            # 曲線データを取得する
            curve_data = curve.data

            # ベジェ曲線の制御点の総数を取得する
            control_point_count = len(curve_data.splines[0].bezier_points)
            
            #モデルオブジェクトをフォローパスに設定する
            fp = obj.constraints.new('FOLLOW_PATH')
            fp.target = curve
            fp.use_fixed_location = True
            fp.use_curve_follow = True

             # ベジェ曲線のドライバーを設定する
            fcurve = fp.driver_add("offset_factor")
            fcurve.driver.expression = "(frame / %d) %% 1" % (50*control_point_count)

            #['file_name']カスタムプロパティを追加
            obj["file_name"] = "move"

        objects = [object for object in context.selected_objects]

        #パネルから得た座標を代入
        for object in objects:
            object.location.x += self.val_x
            object.location.y += self.val_y
            object.location.z += self.val_z

        # ファイルパスの文字列を空に設定する
        context.scene.filepath = ""
        return {'FINISHED'}
    
class ADDONNAME_PT_EnemyPanel(bpy.types.Panel):
    bl_label = "敵を選択"
    bl_idname = "ADDONNAME_PT_EnemyPanel"
    bl_space_type = "VIEW_3D"
    bl_region_type = 'UI'
    bl_category = "Import"
    bl_options = {'DEFAULT_CLOSED'}
    
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