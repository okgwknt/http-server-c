# web-server-c

multi thread は未完

## Description

C 言語で作成した簡易的な HTTP サーバです．

一つのみの接続を受け付ける simple_web_server，マルチプロセスの multi_process_web_server，マルチスレッドの,multi_thread_web_server の 3 種類の HTTP サーバがあります．

それぞれの性能を調べるには httperf を使ってください．

Mac の場合は

```zsh
brew install httperf
```

でインストールすることができます．

詳しくは[こちら](https://github.com/httperf/httperf)を参照してください

## Usage

各ディレクトリで

```zsh
make
```

をすると web_server が build されます．

build したあとは

```zsh
./web_server [port番号]
```

でサーバを起動することができます．
