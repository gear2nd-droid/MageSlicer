# MageSlicer
MageSlicerは、4軸以上の多軸の3Dプリンタ用の汎用スライサーです。多軸での造形により、造形自由度が向上し、一般的な3軸の3Dプリンタでは困難な造形を可能にします。
MAGEは、Multi Axis Generative Engineの略称であり、その名の通り、多軸でのGコードを生成する処理を表しています。
NURBS曲面の造形物を多数のNURBS曲面でスライスします。この際に、法線ベクトルの計算や、層間の間隔なども併せて計算することで、きれいな造形を可能とします。
また、サブボリュームに対応することで、形状に合わせたスライス設定が可能になっています。

## 開発環境
・Visual Studio Community 2022 (17.10.5)

## 使用ライブラリ
・[OpenCASCADE](https://dev.opencascade.org/)

・[Clipper2](https://github.com/AngusJohnson/Clipper2)

