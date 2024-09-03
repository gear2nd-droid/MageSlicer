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

# 使用方法
MAGE slicerでは、スライスしてGコードを直接生成するわけではなく、Gコードを生成するための情報を集めた中間ファイルを生成します。
その中間ファイルをMAGE simulatorに読み込むことで、多軸3Dプリンタの機構を考慮したGコードへの変換と、衝突検出処理を行います。
ポストプロセッサとシミュレータをあえて別に切り離しているのは、多軸の3Dプリンタには様々な機構があることに加え、まだ開発段階のものであり仕様が未確定で頻繁に変更が発生するため、改善を容易にするためです。

# 開発環境
・Visual Studio Community 2022 (17.10.5)

## 使用ライブラリ
・[OpenCASCADE](https://dev.opencascade.org/)

・[Clipper2](https://github.com/AngusJohnson/Clipper2)

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
