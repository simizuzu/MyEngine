import bpy

print('-------')

bpy.data.curves['BezierCurve'] = 

for cv in bpy.data.curves:
    
    if cv.users == 1
        for sp in cv.splines:

            #sp = cv.splines[0]
            print('----------------------')

            # spでもOK. curveを取得してsplinesの0にいってそこからいろいろ調べられるみたい。
            print(len(sp.bezier_points)) # print number of points
            print("idName:" + str(cv.name))
            
            for id, p in enumerate(sp.bezier_points):
                
                print("id:" + str(id))
                print("co:" + str(p.co)) # print coordinate of first point