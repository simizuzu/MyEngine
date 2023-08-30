import bpy
from math import sin, cos, pi

obj = bpy.data.objects['Cube']
obj.animation_data_clear() # アニメーションのデータをクリアする

bpy.context.scene.frame_start = 0
bpy.context.scene.frame_end = 120 # フレームのスタートとエンドを設定する

keyframe_freq = 10 # とりあえず10フレームおきに打ってみる

nStart = bpy.context.scene.frame_start
nEnd = bpy.context.scene.frame_end

for t in range(nStart, nEnd + 1):
    if t % keyframe_freq != 0: continue
    bpy.context.scene.frame_set(t)
    # 位置を変える場合はlocationでアクセスする感じ
    obj.location.x = 3 * cos(t * 0.1)
    obj.location.y = 3 * sin(t * 0.1)
    obj.keyframe_insert(data_path="location")
    # オイラー角をいじるのでrotation_euler. rotationだと失敗する
    obj.rotation_euler.y = t * 0.1
    obj.rotation_euler.z = t * 0.06
    obj.keyframe_insert(data_path="rotation_euler")

    print("----------------------------")
    print("nowFrame:" + str(t))
    print("translation.x:" + str(obj.location.x))
    print("translation.y:" + str(obj.location.y))
    print("translation.z:" + str(obj.location.z))
    print("rotation.x:" + str(obj.rotation_euler.x))
    print("rotation.y:" + str(obj.rotation_euler.y))
    print("rotation.z:" + str(obj.rotation_euler.z))
    

    
# 実はdata_pathの他にframe=60などのオプションを付けることで、
# frame移動しなくてもキーフレームを打てるが、
# フレーム0の時の値が変わってしまうなど作業しづらくなるのでおすすめしない。
# ただスクリプトで大量にオブジェクトを生成するときなどは重宝するかも。
    
bpy.context.scene.frame_set(0) # フレームを戻しておく