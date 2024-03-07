・実行方法・操作方法
キーボード・マウス	K&M
コントローラー		■
タイトルシーン:
セレクトシーンへ移行	K&M:Gキー,		■:Aボタン
セレクトシーン:
2画面に変移(右画面出現)	K&M:Yキー
左画面だけに変移	K&M:Uキー
右画面だけに変移	K&M:Tキー
2画面に変移(左画面出現)	K&M:Rキー
ゲームシーンへ移動	K&M:Gキー,		■:Aボタン
ゲームシーン:
プレイヤー共通の動き:
移動			K&M:WASDキー,		■:左スティック
ジャンプ		K&M:Spaceキー,		■:Aボタン
ダッシュ		K&M:Shiftキー,		■:Rボタン
視点移動		K&M:マウス移動,		■:右スティック
自由視点		K&M:Fキー+マウス移動,	■:Lボタン+右スティック
邪魔する側:
木箱を呼び出す		K&M:マウス右クリック,	■:Yボタン

・フォルダ構成
W:.
│  .gitattributes
│  .gitignore
│  Bone.cpp
│  Bone.h
│  Button.cpp
│  Button.h
│  Dog_Big_Fight.exe
│  Dog_Big_Fight.sln
│  Dog_Big_Fight.vcxproj
│  Dog_Big_Fight.vcxproj.filters
│  Dog_Big_Fight.vcxproj.user
│  Floor.cpp
│  Floor.h
│  GameOverScene.cpp
│  GameOverScene.h
│  GameTitleScene.cpp
│  GameTitleScene.h
│  NormalMapping.hlsl
│  ObjectBase.h
│  ObjectManager.cpp
│  ObjectManager.h
│  Player.cpp
│  Player.h
│  PlayerBase.h
│  PlayScene.cpp
│  PlayScene.h
│  SelectScene.cpp
│  SelectScene.h
│  Sky.cpp
│  Sky.h
│  SolidText.cpp
│  SolidText.h
│  Stage.cpp
│  Stage.h
│  WoodBox.cpp
│  WoodBox.h
│
├─Assets
│  │  AttackDog.fbx
│  │  Bone.fbx
│  │  char.png
│  │  CollectDog.fbx
│  │  defaultParticle.png
│  │  DogWalk.fbx
│  │  Floor.fbx
│  │  FoodButtonGive.png
│  │  FoodButtonGiveDisable.png
│  │  GameOverText.fbx
│  │  GameOverText.png
│  │  GameTitleText.fbx
│  │  GameTitleText.png
│  │  Ground.fbx
│  │  imgui.ini
│  │  innu.fbx
│  │  Player1Win.png
│  │  Player2Win.png
│  │  rocky_trail_02_diff_4k.jpg
│  │  rocky_trail_02_nor_gl_4k.jpg
│  │  SelectText.fbx
│  │  SelectText.png
│  │  setup.ini
│  │  Sky.fbx
│  │  Sky.png
│  │  SkySphere.fbx
│  │  WoodBox.fbx
│  │  wood_planks_diff_4k.jpg
│  │  wood_planks_nor_gl_4k.jpg
│  │
│  ├─.mayaSwatches
│  │      GameOverText.png_hcm.swatch
│  │      GameTitle.png_hcm.swatch
│  │      GameTitleText.png_hcm.swatch
│  │      Ground.png_hcm.swatch
│  │      SelectText.png_hcm.swatch
│  │      Sky.png_hcm.swatch
│  │
│  ├─DebugCollision
│  │      BoxCollider.fbx
│  │      SphereCollider.fbx
│  │
│  └─Shader
│          BillBoard.hlsl
│          Debug3D.hlsl
│          Simple2D.hlsl
│          Simple3D.hlsl
│          Sky.hlsl
│          ToonOutLine.hlsl
│
├─Engine
│  │  Audio.cpp
│  │  Audio.h
│  │  BillBoard.cpp
│  │  BillBoard.h
│  │  BoxCollider.cpp
│  │  BoxCollider.h
│  │  Camera.cpp
│  │  Camera.h
│  │  Collider.cpp
│  │  Collider.h
│  │  CsvReader.cpp
│  │  CsvReader.h
│  │  Debug.cpp
│  │  Debug.h
│  │  Direct3D.cpp
│  │  Direct3D.h
│  │  Fbx.cpp
│  │  Fbx.h
│  │  FbxParts.cpp
│  │  FbxParts.h
│  │  GameObject.cpp
│  │  GameObject.h
│  │  Global.h
│  │  Image.cpp
│  │  Image.h
│  │  Input.cpp
│  │  Input.h
│  │  Main.cpp
│  │  Model.cpp
│  │  Model.h
│  │  RootObject.cpp
│  │  RootObject.h
│  │  SceneManager.cpp
│  │  SceneManager.h
│  │  SphereCollider.cpp
│  │  SphereCollider.h
│  │  Sprite.cpp
│  │  Sprite.h
│  │  Text.cpp
│  │  Text.h
│  │  Texture.cpp
│  │  Texture.h
│  │  Transform.cpp
│  │  Transform.h
│  │  VFX.cpp
│  │  VFX.h
│  │
│  └─ImGui
│          imconfig.h
│          imgui.cpp
│          imgui.h
│          imgui_draw.cpp
│          imgui_impl_dx11.cpp
│          imgui_impl_dx11.h
│          imgui_impl_win32.cpp
│          imgui_impl_win32.h
│          imgui_internal.h
│          imgui_tables.cpp
│          imgui_widgets.cpp
│          imstb_rectpack.h
│          imstb_textedit.h
│          imstb_truetype.h
│
└─x64
    ├─Debug
    │  │  Audio.obj
    │  │  BillBoard.cso
    │  │  BillBoard.obj
    │  │  Bone.obj
    │  │  BoxCollider.obj
    │  │  Button.obj
    │  │  Camera.obj
    │  │  Collider.obj
    │  │  CsvReader.obj
    │  │  Debug.obj
    │  │  Debug3D.cso
    │  │  Direct3D.obj
    │  │  Dog_Big_Fight.Build.CppClean.log
    │  │  Dog_Big_Fight.exe
    │  │  Dog_Big_Fight.exe.recipe
    │  │  Dog_Big_Fight.ilk
    │  │  Dog_Big_Fight.log
    │  │  Dog_Big_Fight.pdb
    │  │  Dog_Big_Fight.vcxproj.FileListAbsolute.txt
    │  │  Fbx.obj
    │  │  FbxParts.obj
    │  │  Floor.obj
    │  │  GameBaseDx11.Build.CppClean.log
    │  │  GameBaseDx11.exe.recipe
    │  │  GameBaseDx11.log
    │  │  GameBaseDx11.vcxproj.FileListAbsolute.txt
    │  │  GameObject.obj
    │  │  GameOverScene.obj
    │  │  GameTitleScene.obj
    │  │  Image.obj
    │  │  imgui.obj
    │  │  imgui_draw.obj
    │  │  imgui_impl_dx11.obj
    │  │  imgui_impl_win32.obj
    │  │  imgui_tables.obj
    │  │  imgui_widgets.obj
    │  │  Input.obj
    │  │  Main.obj
    │  │  Model.obj
    │  │  NormalMapping.cso
    │  │  ObjectManager.obj
    │  │  Player.obj
    │  │  PlayScene.obj
    │  │  RootObject.obj
    │  │  SceneManager.obj
    │  │  SelectScene.obj
    │  │  Simple2D.cso
    │  │  Simple3D.cso
    │  │  Sky.cso
    │  │  Sky.obj
    │  │  SolidText.obj
    │  │  SphereCollider.obj
    │  │  Sprite.obj
    │  │  Stage.obj
    │  │  Text.obj
    │  │  Texture.obj
    │  │  ToonOutLine.cso
    │  │  Transform.obj
    │  │  vc143.idb
    │  │  vc143.pdb
    │  │  VFX.obj
    │  │  WoodBox.obj
    │  │
    │  ├─Dog_Big_Fight.tlog
    │  │      CL.command.1.tlog
    │  │      Cl.items.tlog
    │  │      CL.read.1.tlog
    │  │      CL.write.1.tlog
    │  │      Dog_Big_Fight.lastbuildstate
    │  │      fxc.command.1.tlog
    │  │      fxc.read.1.tlog
    │  │      fxc.write.1.tlog
    │  │      link.command.1.tlog
    │  │      link.read.1.tlog
    │  │      link.write.1.tlog
    │  │
    │  └─GameBaseDx11.tlog
    │          CL.command.1.tlog
    │          Cl.items.tlog
    │          CL.read.1.tlog
    │          CL.write.1.tlog
    │          fxc.command.1.tlog
    │          fxc.read.1.tlog
    │          fxc.write.1.tlog
    │          GameBaseDx11.lastbuildstate
    │          link.command.1.tlog
    │          link.read.1.tlog
    │          link.write.1.tlog
    │
    └─Release
        │  Audio.obj
        │  BillBoard.cso
        │  BillBoard.obj
        │  BoxCollider.obj
        │  Button.obj
        │  Camera.obj
        │  Collider.obj
        │  CsvReader.obj
        │  Debug.obj
        │  Debug3D.cso
        │  Direct3D.obj
        │  Fbx.obj
        │  FbxParts.obj
        │  Floor.obj
        │  GameBaseDx11.exe
        │  GameBaseDx11.exe.recipe
        │  GameBaseDx11.iobj
        │  GameBaseDx11.ipdb
        │  GameBaseDx11.log
        │  GameBaseDx11.pdb
        │  GameBaseDx11.vcxproj.FileListAbsolute.txt
        │  GameObject.obj
        │  GameOverScene.obj
        │  Image.obj
        │  imgui.obj
        │  imgui_draw.obj
        │  imgui_impl_dx11.obj
        │  imgui_impl_win32.obj
        │  imgui_tables.obj
        │  imgui_widgets.obj
        │  Input.obj
        │  Item.obj
        │  Main.obj
        │  Model.obj
        │  NormalMapping.cso
        │  ObjectManager.obj
        │  Player.obj
        │  PlayScene.obj
        │  RootObject.obj
        │  SceneManager.obj
        │  SelectScene.obj
        │  Simple2D.cso
        │  Simple3D.cso
        │  Sky.cso
        │  Sky.obj
        │  SolidText.obj
        │  SphereCollider.obj
        │  Sprite.obj
        │  Stage.obj
        │  Text.obj
        │  Texture.obj
        │  ToonOutLine.cso
        │  Transform.obj
        │  vc143.pdb
        │  VFX.obj
        │  WoodBox.obj
        │
        └─GameBaseDx11.tlog
                CL.command.1.tlog
                Cl.items.tlog
                CL.read.1.tlog
                CL.write.1.tlog
                fxc.command.1.tlog
                fxc.read.1.tlog
                fxc.write.1.tlog
                GameBaseDx11.lastbuildstate
                link.command.1.tlog
                link.read.1.tlog
                link.write.1.tlog
                link.write.2u.tlog
・アピールポイント
画面分割を利用した2人プレイとプレイヤーの動きを時間をかけて作成しました！
特にプレイヤーの地面の坂を上り下りできて、ジャンプや自由落下も再現できるようにしました。Player.cppのPlayerRayCast()関数とPlayerMove()関数内の処理を見てほしいです！
また、Direct3D.cppのUpdate()関数内のビューポートの処理も工夫して作成しました！
