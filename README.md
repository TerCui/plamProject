# plamProject

QT 界面程序
环境：
rv1109/1126开发板+配套双目摄像头
Qt5.14.2

V0.1更新如下：
1、camerathread更新同时打开ir和rgb摄像头获取帧数据
2、新增rgbcamerathread打开单个rgb摄像头进行掌纹算法运算（算法暂时未加入）
3、摄像头帧数据通过opencv的remap进行鱼眼矫正

