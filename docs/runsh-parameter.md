# runsh-parameter.md

## run.sh format

#Gloable Parameter Setting:GP
#GPリスト：TOOLMODE, TPG, TEST_VEC, CIRCUIT

##TOOLMODE:シミュレーションの動作目的を指定する
#=1: Normal Scan Test 通常のスキャンテストモード（シングルキャプチャ）
#=2: Multi-cycle Test, 通常のマルチサイクルテストモード、キャプチャ数指定が必要
#CAPTURE: マルチサイクルテスト動作モード時のキャプチャ数、最大５０まで指定可
#=3: Multi-cycle Test with SeqOB,マルチサイクルテスト中間観測する動作モード
#キャプチャ数、ＦＦ観測率（全体ＦＦ数の比率）、観測用ＦＦランキングリストパスの指定が必要
#CAPTURE: マルチサイクルテスト動作モード時のキャプチャ数、最大５０まで指定可
#OBRATE: ＦＦ観測比率（0~1の小数）
#SKIPCYCLES: 観測しないサイクル番号、CAPTURE=10, SKIPCYCLES=3の場合、１０サイクルテストにおいて、第３サイクルからＦＦ値を観測する
#ff_sta_file: 観測用ＦＦランキングリストファイルパス指定
#=4: マルチサイクルテストwithＣＰＩ、ＦＦ－ＣＰＩやLogic-CPIはパラメータで指定する

##TPG: テストパターン生成モード、=0: LFSR, =1: ATPG
##TEST_VEC: テストパターン数を指定する。

TPG=0 #=0:LFSR,=1:ATPG
TEST_VEC=10 #0 00 # Number of Test patterns
TOOLMODE=3 #=1:Normal Scan test, =2:Multi-cycle Test, =3: Multi-cycle test with Seq OB, =4:Toggle Gate TPI

OBRATE=0.2 #Observation Rate
SKIPCYCLES=0
SELECTSTATION=3 #0: SCOAP, 1: cop+type, 2:OP 3: multi observation