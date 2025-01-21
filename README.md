# MAGE slicer
MAGE slicerは、4軸以上の多軸の3Dプリンタ用の汎用スライサーです。
多軸の3Dプリンタは多軸での造形により、造形自由度が向上し、一般的な3軸の3Dプリンタでは困難な造形を可能にします。
MAGE slicerでは、NURBS曲面を用いて計算することで、高精度な自由曲面でのスライスを実現しています。
そのため、自由曲面上での3次元座標に加え、法線ベクトルの計算や、層間の間隔なども併せて計算することで綺麗な造形を可能とします。
なお、3軸の3Dプリンタで非平面印刷をするのにも活用できます。

## MAGEとは?
MAGEは、Multi Axis Generative Engineの略称です。
その名の通り、多軸でのGコードを生成する処理を表しています。

## MAGE Printer
MAGE Printerは、MAGE Slicerに対応した5軸3Dプリンタです。
[Kindle Direct Publishing](https://www.amazon.com/dp/B0DTGR7GW4)にて、製作方法をまとめた書籍を販売しております。

# スライス手法
MAGE slicerでは現在3種類のスライス方式に対応しています。

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

## 実装予定
- サポート材生成
- 自動でのサブボリューム分割とサンドイッチスライス用の曲面生成

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

## 操作工程
1. スライスする設定を行います。
2. 「Pre slice」でスライスする曲面を生成・読込します。
3. シークバーを操作し、スライスする曲面が正常に生成されているか確認します。
4. 「Slice」で実際にスライスを行い、多数の点群およびその各点ごとの付加情報をcsvファイルとして出力します。

## 3D描画画面の操作
- メニューバー左端:描画しているコンテンツに合わせてフィッティング。
- 左ドラッグ:視点移動
- 上下に中ドラッグ:ズームイン・ズームアウト
- 右ドラッグ:注視点移動

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

# 入力データ
スライス対象物をSTEPファイルで、スライスに用いるNURBS曲面をCSVファイルで読み込みます。
このうち、CSVファイルは独自フォーマットとなっています。
また、スライス設定はXMLファイルでプロジェクトとして保存することができます。

## CSVファイルのフォーマット
NURBS曲面は、UV方向に配列された点群の座標から生成することができます。
```
2 // 2が固定値です。UVでマッピングされた座標を読み込むことを意味しています。
4,10 // Uの数、Vの数です。
-15.000,-15.000,-15.000,-15.000 // 列でU方向を、行でV方向を表し、X座標のみのリストです。
-11.667,-11.667,-11.667,-11.667
-8.333,-8.333,-8.333,-8.333
-5.000,-5.000,-5.000,-5.000
-1.667,-1.667,-1.667,-1.667
1.667,1.667,1.667,1.667
5.000,5.000,5.000,5.000
8.333,8.333,8.333,8.333
11.667,11.667,11.667,11.667
15.000,15.000,15.000,15.000
-13.000,-4.000,5.000,14.000 // 列でU方向を、行でV方向を表し、Y座標のみのリストです。
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
-13.000,-4.000,5.000,14.000
83.000,83.000,83.000,83.000 // 列でU方向を、行でV方向を表し、Z座標のみのリストです。
71.779,71.779,71.779,71.779
64.344,64.344,64.344,64.344
60.037,60.037,60.037,60.037
58.199,58.199,58.199,58.199
58.199,58.199,58.199,58.199
60.037,60.037,60.037,60.037
64.344,64.344,64.344,64.344
71.779,71.779,71.779,71.779
83.000,83.000,83.000,83.000
// でコメントを記載しましたが、コメントは非対応です。
```

## XMLファイルのフォーマット
```
<?xml version="1.0" encoding="UTF-8"?>
<Project>
  <Targets>
    <Target Path="C:\test\bottom_sandwich_test.stp" RotAngle="0" RotCenterX="0" RotCenterY="0" RotCenterZ="0" RotDirectionX="0" RotDirectionY="0" RotDirectionZ="1" TransX="0" TransY="0" TransZ="0" />
    <Target Path="C:\test\piller_test.stp" RotAngle="0" RotCenterX="0" RotCenterY="0" RotCenterZ="0" RotDirectionX="0" RotDirectionY="0" RotDirectionZ="1" TransX="0" TransY="0" TransZ="0" />
  </Targets>
  <Surfaces>
    <Surface Path="C:\test\bottom.csv" RotAngle="0" RotCenterX="0" RotCenterY="0" RotCenterZ="0" RotDirectionX="0" RotDirectionY="0" RotDirectionZ="1" TransX="0" TransY="0" TransZ="0" />
    <Surface Path="C:\test\upper.csv" RotAngle="0" RotCenterX="0" RotCenterY="0" RotCenterZ="0" RotDirectionX="0" RotDirectionY="0" RotDirectionZ="1" TransX="0" TransY="0" TransZ="0" />
  </Surfaces>
  <Volumes>
    <Volume Type="Bottom" TargetId="0" SurfaceId="0" Direction="0" Thickness = "0.4"/><!--Directionは、0:U方向から90度、1:U方向から0度、2:U方向から45度、3:U方向から-45度の傾きの平行線で、外層と内層の間を埋めます。-->
    <Volume Type="Sandwich" TargetId="0" LowerId="0" UpperId="1" Pitch="0.15" /><!--Pitchは生成する補間曲面の間隔を決めるパラメータであって、この値が厳密な間隔になるわけではありません。-->
    <Volume Type="Piller" TargetId="1" Thickness="0.4" Direction="0" /><!--Directionは、0:U方向から45度、1:V方向から-45度の傾きの平行線で、層を埋めます。-->
  </Volumes>
</Project>
```

