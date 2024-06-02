import bpy
import math
import bpy_extras
import gpu
import gpu_extras.batch
import copy
import mathutils
import json
import os

# アドオンのメタデータ
bl_info = {
    "name": "GetAnimationData",  # アドオンの名前
    "author": "Sho Shimizu",  # 作者名
    "version": (1, 0),  # バージョン
    "blender": (3, 1, 2),  # 対応するBlenderのバージョン
    "location": "View3D > Sidebar > Animation Keyframes",  # アドオンのUI位置
    "description": "アニメーションのキーフレームをjsonファイルにて出力する",  # 説明
    "warning": "",  # 警告メッセージ
    "support": "TESTING",  # サポート状況
    "wiki_url": "",  # ドキュメントURL
    "tracker_url": "",  # バグ追跡URL
    "category": "Animation"  # カテゴリ
}

# オペレータクラス：アニメーションキーフレームをJSONファイルに出力する
class OBJECT_OT_PrintAnimationKeyframes(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    bl_idname = "object.print_animation_keyframes"  # オペレータID
    bl_label = "Print Animation Keyframes"  # オペレータラベル
    bl_description = "オブジェクトのアニメーションデータ取得します"  # オペレータの説明
    bl_options = {'REGISTER', 'UNDO'}  # オプション（登録とアンドゥ可能）

    # 出力するファイルの拡張子
    filename_ext = ".json"
    
    def export_json(self):
        """JSON形式でファイルに出力"""
        # 保存する情報をまとめるdict
        json_object_root = dict()
        # ノード名
        json_object_root["name"] = "scene"
        # オブジェクトリストを作成
        json_object_root["animations"] = list()
                
        # シーン内の全オブジェクトをループ
        for obj in bpy.context.scene.objects:
            # アニメーションデータを取得してJSON形式に整形
            animation_data = self.get_animation_data(obj)
            last_frame = self.get_last_frame(animation_data)
          
            # オブジェクトの情報をまとめる
            obj_data = {
                "type": obj.type,  # オブジェクトの種類
                "name": obj.name,  # オブジェクトの名前
                "last_frame": last_frame,
                "animedata": animation_data,  # オブジェクトのアニメーションデータ
            }
            # 出力データにオブジェクト情報を追加
            json_object_root["animations"].append(obj_data)
        
        # JSONファイルに書き込む
        with open(self.filepath, "w") as file:
            json.dump(json_object_root, file, indent=4, default=self.default_serializer)

    def get_last_frame(self, animation_data):
        """アニメーションデータの最後のフレームを取得"""
        if animation_data:
            return max(item["frame"] for item in animation_data)
        return 0
            

    def get_animation_data(self, obj, include_scale=True):
        """オブジェクトのアニメーションデータを取得"""
        animation_data = []

        # オブジェクトのアニメーションカーブを取得
        fcurves = obj.animation_data.action.fcurves

        # アニメーションカーブをループ
        for fcurve in fcurves:
            if fcurve.data_path.startswith("location"):
                # 位置情報の処理
                self.add_frame_to_animation_data(animation_data, fcurve)
                self.get_location_data(fcurve, animation_data)
            elif fcurve.data_path.startswith("rotation"):
                # 回転情報の処理
                self.add_frame_to_animation_data(animation_data, fcurve)
                self.get_rotation_data(fcurve, animation_data, obj)
            elif fcurve.data_path.startswith("scale") and include_scale:
                # スケール情報の処理
                self.add_frame_to_animation_data(animation_data, fcurve)
                self.get_scale_data(fcurve, animation_data)

        return animation_data

    def add_frame_to_animation_data(self, animation_data, fcurve):
        """フレーム情報を先頭に追加する"""
        for keyframe in fcurve.keyframe_points:
            frame = int(keyframe.co[0])
            # 既存のフレームデータがあるか確認
            index = next((i for i, d in enumerate(animation_data) if d['frame'] == frame), None)
            if index is None:
                animation_data.append({"frame": frame})

    def get_location_data(self, fcurve, animation_data):
        """位置情報を取得"""
        for keyframe in fcurve.keyframe_points:
            frame = int(keyframe.co[0])
            value = keyframe.co[1]
            # 既存のフレームデータがあるか確認
            index = next((i for i, d in enumerate(animation_data) if d['frame'] == frame), None)
            if index is None:
                animation_data.append({"frame": frame, "translation": [value]})
            else:
                animation_data[index].setdefault("translation", []).append(value)

    def get_rotation_data(self, fcurve, animation_data, obj):
        """回転情報を取得"""
        frame_rotation_map = {}
        for keyframe in fcurve.keyframe_points:
            frame = int(keyframe.co[0])
            bpy.context.scene.frame_set(frame)
            rot_quaternion = obj.rotation_quaternion
            rot_values = [rot_quaternion.w, rot_quaternion.x, rot_quaternion.y, rot_quaternion.z]

            if frame not in frame_rotation_map:
                frame_rotation_map[frame] = rot_values

        for frame, rotation in frame_rotation_map.items():
            index = next((i for i, d in enumerate(animation_data) if d['frame'] == frame), None)
            if index is None:
                animation_data.append({"frame": frame, "rotation": rotation})
            else:
                # 回転情報が既に存在しない場合に追加
                if "rotation" not in animation_data[index]:
                    animation_data[index]["rotation"] = rotation

    def get_scale_data(self, fcurve, animation_data):
        """スケール情報を取得"""
        for keyframe in fcurve.keyframe_points:
            frame = int(keyframe.co[0])
            value = keyframe.co[1]
            # 既存のフレームデータがあるか確認
            index = next((i for i, d in enumerate(animation_data) if d['frame'] == frame), None)
            if index is None:
                animation_data.append({"frame": frame, "scaling": [value]})
            else:
                animation_data[index].setdefault("scaling", []).append(value)
 
    def execute(self, context):
        """ファイルに出力"""
        print("シーン情報出力開始... %r" % self.filepath)     
        # ファイルに出力
        self.export_json()      
        self.report({'INFO'}, "シーン情報をExportしました")
        print("シーン情報をExportしました")
       
        # オペレータの命令終了を通知
        return {'FINISHED'}
    
    def default_serializer(obj):
        """JSONシリアライザ"""
        if isinstance(obj, mathutils.Euler):
            return [math.degrees(angle) for angle in obj]
        raise TypeError(f"Object of type '{obj.__class__.__name__}' is not JSON serializable")
    
# UIパネルクラス：アニメーションキーフレームの出力ボタンを表示
class OBJECT_PT_AnimationKeyframesPanel(bpy.types.Panel):
    bl_label = "Animation Keyframes"  # パネルラベル
    bl_idname = "OBJECT_PT_animation_keyframes"  # パネルID
    bl_space_type = 'VIEW_3D'  # 表示スペース（3Dビュー）
    bl_region_type = 'UI'  # 表示リージョン（UI）
    bl_category = 'Animation'  # カテゴリ
    
    def draw(self, context):
        layout = self.layout
        layout.operator("object.print_animation_keyframes", text="Print Keyframes")

# Blenderに登録するクラスリスト
classes = {
    OBJECT_OT_PrintAnimationKeyframes,
    OBJECT_PT_AnimationKeyframesPanel,
}

# アドオン有効時コールバック
def register():
    # Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)
    bpy.types.VIEW3D_MT_object.append(OBJECT_OT_PrintAnimationKeyframes)
    bpy.types.VIEW3D_MT_object.append(OBJECT_PT_AnimationKeyframesPanel)  
    # メニューに項目を追加
    print("レベルエディタが有効化されました。")

# アドオン無効時コールバック
def unregister():
    bpy.types.VIEW3D_MT_object.remove(OBJECT_OT_PrintAnimationKeyframes)
    bpy.types.VIEW3D_MT_object.remove(OBJECT_PT_AnimationKeyframesPanel)
    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効時されました。")
