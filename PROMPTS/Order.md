あなたはプログラミングのアドバイザーです。
常にベストプラクティスを意識しながらスマートなコードを書くようにします。

# 作成物と開発経緯
m5stack core s3を使って、発注・納品を効率的に行えるデバイスおよびアプリケーションを開発しています。
これまであなたの助言を聞きつつ、開発を進めてきました。
コードは随時githubにプッシュしています。https://github.com/manusa3190/QRcodeOrder

# あなたへの要求
1. 私の質問に対して、日本語で回答してください
2. 回答を考えるときは、あなたの知識だけで回答するのではなく、githubのリポジトリにてファイル全体を読み取りつつ、ベストな方法を提示してください

# 今回の作成物と目標
注文機能を実装するため、Order.h及びOrder.cppを更新します。
1. Order.cppではユーザーが発注可能なアイテムのリストを表示します。
2. ユーザーは必要なアイテムの行をタッチします
3. そうするとダイアログにそのアイテムのコードを示すQRコードが表示されます。

# 内容
## 1. 発注可能なアイテムのリストを取得
Orderページが開かれると、M5stackは以下URLにGETリクエストを送ります。

https://www.appsheet.com/api/v2/apps/7b6b4413-ec3a-4d5d-b9d0-918c175e4487/tables/備品マスタ/Action?applicationAccessKey=V2-hapij-FMGbT-1rmp9-rEdZ6-BAF9C-UNfTv-mxMl1-gwaPa

この時、以下のpayloadおよびoptionsを付けます。（コードはjavascriptなので、適宜cppに変換してください）

payload = {
"Action":"Find",
"Properties":{
    "Locale":"ja-JP",
    "Selector": "FILTER('備品マスタ', IN('実験室', [格納場所]))"
}
}

options = {
    "method":"post",
    "contentType":"application/json",
    "payload":JSON.stringify(payload)
}

リクエストが正常にできると

## 2.