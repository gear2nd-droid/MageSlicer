# MAGE slicer
MAGE slicerは、4軸以上の多軸の3Dプリンタ用の汎用スライサーです。
多軸の3Dプリンタは多軸での造形により、造形自由度が向上し、一般的な3軸の3Dプリンタでは困難な造形を可能にします。
MAGE slicerでは、NURBS曲面を用いて計算することで、高精度な自由曲面でのスライスを実現しています。
そのため、自由曲面上での3次元座標に加え、法線ベクトルの計算や、層間の間隔なども併せて計算することで綺麗な造形を可能とします。

## MAGEとは?
MAGEは、Multi Axis Generative Engineの略称です。
その名の通り、多軸でのGコードを生成する処理を表しています。

# スライス手法
MAGE slicerでは現在1(+1)種類のスライス方式に対応しています。

## サンドイッチスライス
異なる2つのNURBS曲面を設定し、その間を適宜補完したNURBS曲面を生成してスライスします。
基本的には、法線方向にノズルを持っていくように制御することができます。
インフィルの生成に対応しています。

## ボトムスライス
底面を塗りつぶすためのスライスです。
一般的な3Dプリンタのスライサーと同様に外層・内層に接する部分を一筆書きしたうえで、内側がジグザグで塗りつぶします。
底面にのみ対応しています。

## ピーラースライス
外皮のみを一層と見立ててスライスします。
そのため、一般的な3Dプリンタでは水平面内を鉛直方向に積層していくだけですが、その積層面間を接続するような新たな面を造形することができます。

# 実装済み機能
- 中空:内部に空洞を有した造形物を作成可能です。
- 複数層の壁:外壁および内壁を複数層とすることができます。
- インフィル:インフィルを作成することができます。インフィルはXYZ空間基準とNURBS曲面内のUV空間基準で作成することができます。
- オーバーハング箇所の補間:レイヤー間で急激にスライス形状が変化した箇所では、隙間が発生してしまうことがあります。この隙間を埋める機能があります。

# 使用方法
MAGE slicerでは、スライスしてGコードを直接生成するわけではなく、Gコードを生成するための情報を集めた中間ファイルを生成します。
その中間ファイルをMAGE simulatorに読み込むことで、多軸3Dプリンタの機構を考慮したGコードへの変換と、衝突検出処理を行います。
ポストプロセッサとシミュレータをあえて別に切り離しているのは、多軸の3Dプリンタには様々な機構があることに加え、まだ開発段階のものであり仕様が未確定で頻繁に変更が発生するため、改善を容易にするためです。

## 画面構成
![スクリーンショット 2025-01-12 104001](https://github.com/user-attachments/assets/e0d7fb3c-161f-4252-b87a-c25e6cd1b3b8)
- 左ペインの上段には、スライス対象や、スライスに用いる曲面を描画して表示しています。
- 右ペインの上段には、設定しているスライス設定が表示されています。
- 右ペインの下段には、選択している右ペインの上段の詳細を表示しています。
- 左ペインの下段には、スライス状況などの情報が表示されます。
- メニューバーには、画面の表示状態を操作するボタンが配置されています。

# 開発環境
- Visual Studio Community 2022 (17.10.5)

## 使用ライブラリ
- [OpenCASCADE](https://dev.opencascade.org/)
- [Clipper2](https://github.com/AngusJohnson/Clipper2)

## 環境設定方法
使用しているライブラリはvcpkgでインストール可能です。
VisualStudioをインストール後に、コマンドプロンプトでvcpkgをインストールしてください。
なお、Gitのインストールにはコマンドプロンプトを管理者として実行することが必要です。
```
winget install --scope machine Git.Git
powershell -command "$oldpath = [System.Environment]::GetEnvironmentVariable(\"Path\", \"Machine\"); $oldpath += \";c:\Program Files\Git\cmd\"; [System.Environment]::SetEnvironmentVariable(\"Path\", $oldpath, \"Machine\")"

cd c:\
rmdir /s /q vcpkg
git clone https://github.com/microsoft/vcpkg
cd c:\vcpkg
git pull
.\bootstrap-vcpkg.bat
.\vcpkg.exe integrate install

.\vcpkg install opencascade
.\vcpkg install clipper2
```

MageSlicer.slnを起動し、OCCTProxyを右クリックしてプロパティ。
構成プロパティ>vcpkg>General>Use Vcpkgを「はい」に設定する。

いくつかのファイルで「AboutDialog.resx を処理できませんでした。インターネットまたは制限付きゾーン内にあるか、ファイルに Web のマークがあるためです。これらのファイルを処理するには、Web のマークを削除してください。」というエラーがでます。
エクスプローラで該当のファイルを右クリックしプロパティ。
全般>セキュリティ>許可するに「チェック」を入れてください。

# 中間ファイル
出力される中間ファイルはヘッダがないcsvファイルです。この中間ファイルを[MAGE Interface](https://github.com/gear2nd-droid/MageSimulator)でGコードに変換して3Dプリンタを動かします。

## フォーマット
1. 種類(Type)
2. X座標(X)
3. Y座標(Y)
4. Z座標(Z)
5. 法線ベクトルのX成分(I)
6. 法線ベクトルのY成分(J)
7. 法線ベクトルのZ成分(K)
8. 厚み(Thickness)
9. 幅(Width)
10. ボリュームID
11. レイヤーID
12. ループID
13. カーブID
14. NURBS曲面のUパラメータ
15. NURBS曲面のVパラメータ

## 種類
11. OuterWallMiddle
12. OuterWallStart
13. OuterWallEnd
21. InnerWallMiddle
22. InnerWallStart
23. InnerWallEnd
31. InfillMiddle
32. InfillStart
33. InfillEnd
41. SupportMiddle
42. SupportStart
43. SupportEnd
51. Saving
