# 109503003 陳柏禎 通訊三
# Compile C code
進到程式所在資料夾  
1.輸入make  
2.接下來看要執行下列哪個動作  
open a new game: ./main -n filename.txt  
load a game: ./main -l filename.txt
## 開新局
1.依照提示依序輸入座標位置  
2.若要儲存檔案則輸入s（注意：若沒有按下s則不會有儲存對局，考慮到不是每一局使用者都想要存取）  
3.按0悔棋  
4.每次輸入完畢紅藍方會對調以方便使用者視角
## 讀取棋譜
輸入f往下一步 輸入b往回一步  
ps:內部有一檔案new.txt是現成的棋譜，方便助教測試使用

# 結束遊戲
請直接按control+c
