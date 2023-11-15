import bpy
import math
import bpy_extras
import json

#ブレンダーに登録するアドオン情報
bl_info = {
    "name": "キーフレームエディタ",
    "author": "Sho Shimizu",
    "version": (1, 0),
    "blender": (3, 1, 2),
    "location": "",
    "description": "キーフレームエディタ",
    "warning": "",
    "support": "TESTING",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Object"
}
    
#オペレータ シーン出力
class MYADDON_OT_export_animdata(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_animdata"
    bl_label = "キーフレーム出力"
    bl_description = "キーフレーム情報をExportします"

    #出力するファイルの拡張子
    filename_ext = ".json"

    def write_and_print(self, file, str):
        print(str)

        file.write(str)
        file.write('\n')

    def parse_scene_recursive(self, file):
        """シーン解析用再帰関数"""

        #シーン内の全オブジェクトについて
        for object in bpy.context.scene.objects:
       
            location = []
            rotation = []
            totalFrame = []
            count = 0
            lo1 = 0
            lo2 = 0
            lo3 = 0
            ro1 = 0
            ro2 = 0
            ro3 = 0


            for fc in object.animation_data.action.fcurves:
                if fc.data_path.endswith(('location')):
                    for key in fc.keyframe_points :
                        location.append(key.co[1])
                        totalFrame.append(key.co[0])
                        count += 1

                if fc.data_path.endswith(('rotation_euler')):
                    for key2 in fc.keyframe_points :
                        rotation.append(key2.co[1])

            for num in range(count):
                if num % 3 == 0:
                    lo1 = location[num]
                    ro1 = rotation[num]

                if num % 3 == 1:
                    lo2 = location[num]
                    ro2 = rotation[num]

                if num % 3 == 2:
                    lo3 = location[num]
                    ro3 = rotation[num]

                    self.write_and_print(file, 'Frame(' + str(totalFrame[num // 3]) + '):')
                    self.write_and_print(file, "Trans(%f,%f,%f):" % (lo1, lo2, lo3))
                    self.write_and_print(file, "Rot(%f,%f,%f):" % (ro1, ro2, ro3))
                    self.write_and_print(file, '')

    def parse_scene_recursive_json(self, data_parent):

        #シーンのオブジェクト1個分のjsonオブジェクト生成
        
        #オブジェクト種類
        #["type"] = object.type
        #オブジェクト名
        

        #その他の情報をパック
        #シーン内の全オブジェクトについて
        for object in bpy.context.scene.objects:
       
            location = []
            rotation = []
            totalFrame = []
            count = 0
            lo1 = 0
            lo2 = 0
            lo3 = 0
            ro1 = 0
            ro2 = 0
            ro3 = 0
  
            for fc in object.animation_data.action.fcurves:
                if fc.data_path.endswith(('location')):
                    for key in fc.keyframe_points :
                        location.append(key.co[1])
                        totalFrame.append(key.co[0])
                        count += 1

                if fc.data_path.endswith(('rotation_euler')):
                    for key2 in fc.keyframe_points :
                        rotation.append(key2.co[1])

            for num in range(count):
                if num % 3 == 0:
                    lo1 = location[num]
                    ro1 = rotation[num]

                if num % 3 == 1:
                    lo2 = location[num]
                    ro2 = rotation[num]

                if num % 3 == 2:
                    lo3 = location[num]
                    ro3 = rotation[num]

                    json_object = dict()
                    json_object["name"] = object.name
                    transform = dict()
                    #トランスフォーム情報をディクショナリに登録
                    transform["nowframe"] = totalFrame[num // 3]
                    transform["translation"] = (lo1, lo2, lo3)
                    transform["rotation"] = (ro1, ro2, ro3)
                    #まとめて1個分のjsonオブジェクトに登録
                    json_object["keyframe"] = transform

                    #1個分のjsonオブジェクトを親オブジェクトに登録
                    data_parent.append(json_object)

    
    def export(self):
        """ファイルに出力"""

        print("シーン情報出力開始... %r" % self.filepath)

        #ファイルをテキスト形式で書き出し用にオープン
        #スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt") as file:
            #ファイルに文字列を書き込む
            self.parse_scene_recursive(file)
            print()

    def export_json(self):
        """JSON形式でファイルに出力"""

        #保存する情報をまとめるdict
        json_object_root = dict()

        #ノード名
        json_object_root["name"] = "scene"
        #オブジェクトリストを作成
        json_object_root["animdata"] = list()

        self.parse_scene_recursive_json(json_object_root["animdata"])

        #オブジェクトをJSON文字列にエンコード（改行・インデント付き）
        json_text = json.dumps(json_object_root, ensure_ascii=False, cls=json.JSONEncoder, indent=4)

        #コンソールに表示
        print(json_text)

        #ファイルをテキスト形式で書き出しようにオープン
        #スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt", encoding="utf-8") as file:
            #ファイルに文字列を書き込む
            file.write(json_text)
   
    def execute(self, context):

        print("シーン情報をExportします")

        #ファイルに出力
        self.export_json()

        print("シーン情報をExportしました")
        self.report({'INFO'}, "シーン情報をExportしました")

        #オペレータの命令終了を通知
        return {'FINISHED'}

#トップバーの拡張メニュー
class TOPBAR_MT_my_menu(bpy.types.Menu):
    #Blenderがクラスを識別するための固有の文字列
    bl_idname = "TOPBAR_MT_my_menu"
    #メニューのラベルとして表示される文字列
    bl_label = "MyMenuAnim"
    #著者表示用の文字列
    bl_description = "拡張メニュー by " + bl_info["author"]

    #サブメニューの描画
    def draw(self, context):
        #トップバーの「エディターメニュー」に項目（オペレーター）を追加
        self.layout.operator(MYADDON_OT_export_animdata.bl_idname, text=MYADDON_OT_export_animdata.bl_label)

    def submenu(self,context):
        #ID指定でサブメニューを追加
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)

#Blenderに登録するクラスリスト
classes = (
    MYADDON_OT_export_animdata,
    TOPBAR_MT_my_menu,
)

#アドオン有効時コールバック
def register():
    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    print("レベルエディタが有効化されました。")

#アドオン無効時コールバック
def unregister():
    #メニューに項目を追加
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)

    #Blenderにクラスを登録
    for cls in classes:
        bpy.utils.unregister_class(cls)
    print("レベルエディタが無効時されました。")

#テスト実行用コード
if __name__ == "__main__":
    register()