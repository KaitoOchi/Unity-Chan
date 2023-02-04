<a id="anchor1"></a>
# Unity_Chan!
制作：越智海斗

<a id="anchor2"></a>
## 目次
* [Unity_Chan!](#anchor1)
* [目次](#anchor2)
* [概要](#anchor3)<br>
　1. [紹介動画](#anchor4)<br>
　2. [使用ツール](#anchor5)<br>
　3. [使用言語](#anchor6)<br>
　4. [開発期間](#anchor7)<br>
* [操作方法](#anchor8)
* [特徴](#anchor9)<br>
　1. [State](#anchor10)<br>
　2. [基底クラス](#anchor11)<br>
　3. [Levelデザイン](#anchor12)<br>
　4. [動く床](#anchor13)<br>
　5. [壁キック](#anchor14)<br>

<a id="anchor3"></a>
## 概要
  2Dのスクロール型アクションで、特殊武器と特殊アクションを駆使し、<br>
  ステージ最奥にいるボスを撃破するゲームです。

<a id="anchor4"></a>
1.**紹介動画**<br>
<https://youtu.be/QxvxDh62Y8c>

<a id="anchor5"></a>
2.**使用ツール**<br>
Visual Studio 2022<br>
3ds Max 2023<br>
Adobe Photoshop 2022<br>


<a id="anchor6"></a>
3.**使用言語**<br>
C++<br>


<a id="anchor7"></a>
4.**開発期間**<br>
2022.09 ~ 2023.01<br>

<a id="anchor8"></a>
## 操作方法
![image](https://user-images.githubusercontent.com/109284106/213832976-033ceea1-2a82-4b48-b7f1-92c980e166d0.png)

<a id="anchor9"></a>
## 特徴

<a id="anchor10"></a>
### **1. State**<br>
---
  プレイヤーのアクションをStateパターンで書くことにより、ジャンプ、走る、スライディングなどの状態の<br>
  重複を防ぎ、また、アクションを追加する際のコードミスを減らし、視認性を上げる目的があります。
  ![image](https://user-images.githubusercontent.com/109284106/212590520-b2fd050d-3223-457d-8049-a5ac1750b227.png)
  <br>

<a id="anchor11"></a>
### **2. 基底クラス**<br>
---
  プレイヤー、敵、ボスに共通する、「座標」「HP」「方向」などを<br>
  基底クラスActorにまとめ、コードの簡略化をしています。
  ![image](https://user-images.githubusercontent.com/109284106/212590983-78d5c010-3fc2-4640-80e4-c07103b94b6c.png)
  <br>

<a id="anchor12"></a>
### **3. Levelデザイン**<br>
---
  マップに登場する敵やオブジェクト、マップスクロールする座標は、すべて3dsMaxに配置し、<br>
  指定したレベルを読み込むことでクラスを召喚させています。
  ![image](https://user-images.githubusercontent.com/109284106/212591366-83e8552f-9010-422a-b9a9-520fb572243f.png)

<a id="anchor13"></a>
### **4. 動く床**<br>
---
  使用している学内製のエンジンでは、静的物理オブジェクトクラスの座標指定関数が無く、<br>
  また、接地したときにプレイヤーの移動もされないので、動く床を自作する必要がありました。
  ![image](https://user-images.githubusercontent.com/109284106/212591437-4b9b0f2e-d678-4239-8af3-eb09fca0c689.png)<br>
  動く床に触れているとき、変数addMoveSpeedに床の移動方向の値を入れ、<br>
  最終的なプレイヤーの移動速度の計算時に加算することで求めています。
  <br>

<a id="anchor14"></a>
### **5. 壁キック**<br>
---
  このエンジンには他にも、接地判定はありますが、壁に触れたときの判定を出力する関数がなく、<br>
  動く床同様自作する必要があります。
  ![image](https://user-images.githubusercontent.com/109284106/212591579-adb3c9f9-ff25-497b-88d3-b0d908ca8b90.png)
  「壁キックを出す条件」を考えたとき、「移動キーの入力がされている状態」かつ
「プレイヤーの移動がない（座標が変わらない）状態」に、壁キックを出せるようにしました。

