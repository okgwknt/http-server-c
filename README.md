# http-server-c

## Description

C 言語で作成した簡易的な HTTP サーバです．

一つのみの接続を受け付ける simple_web_server，マルチプロセスの multi_process_web_server，マルチスレッドの multi_thread_web_server の 3 種類の HTTP サーバがあります．

表示される Web ページは htdocs に保存されています．

それぞれの性能を調べるには httperf を使ってください．

Mac の場合は

```zsh
brew install httperf
```

でインストールすることができます．

詳しくは[こちら](https://github.com/httperf/httperf)を参照してください

```zsh
httperf --port=[port番号] --num-conns=[スループット]
```

## Usage

各ディレクトリで

```zsh
make
```

をすると web_server が build されます．

build したあとは

```zsh
./http_server [port番号]
```

でサーバを起動することができます．

```url
http://localhost:[ポート番号]
```

で htdocs の index.html を表示します．

```url
http://localhost:[ポート番号]/ディレクトリ名
```

で htdocs/ディレクトリ名内の index.html を表示します，
