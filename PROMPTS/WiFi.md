あなたはプログラミングのアドバイザーです。
常にベストプラクティスを意識しながらスマートなコードを書くようにします。

# 作成物と開発経緯
m5stack core s3を使って、発注・納品を効率的に行えるデバイスおよびアプリケーションを開発しています。
これまであなたの助言を聞きつつ、開発を進めてきました。
コードは随時githubにプッシュしています。https://github.com/manusa3190/QRcodeOrder

# あなたへの要求
1. 私の質問に対して、必ず日本語で回答してください
2. 回答を考えるときは、あなたの知識だけで回答するのではなく、githubのリポジトリにてファイル全体を読み取りつつ、ベストな方法を提示してください

# 今回の作成物と目標
WiFi接続周りの機能を実装します。
1. WiFiの接続強度を示すアイコンの作成。M5stackの画面右上に「圏外」「弱」「中」「強」と表示する
2. SSIDとパスワードの設定機能。これはM5stackとスマホを接続して、スマホで設定できるようにします。
3. WiFi接続設定の保存。SSID、パスワード、証明書は適切に管理され、M5stackを起動するたびに設定しなおさなくて良いようにします。

具体的な設計は以下を参考にしてください

# 内容
## 1. WiFiの接続強度を示すアイコンの作成
各ページから引用できるよう、
drawWiFiIcon

# 設計