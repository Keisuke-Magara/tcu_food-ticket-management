#include <bits/stdc++.h>
#include<iostream>
#include<string>
#include<vector>
#include <array>
#include <fstream>
#define maxnum 128  //食券の種類の最大数
using namespace std;
using std::cout; using std::endl;
using std::vector; using std::string;
std::ofstream writing_file;
std::string filename="recommend.txt";
struct ticket{

              int number;   //
              int nownum;   //今の食券の残り枚数
              double orinum;   //元の食券の枚数
             double jyuppun;   //直近１０分で売れた枚数
             string menu;   //料理名
              int  price;   //値段
             int sellnum;   //売れた枚数
              double percent;	//今売れている枚数の割合
  int perrank;   //売れている割合の順位
  int sellrank; 
            int jyurank;
};
int number(){
 std::ifstream ifs("../ftp_files/tickets.info");
  std::string str;
  int k=0;
  while(getline(ifs,str))
		{
		  //	  std::cout<< str <<std::endl;
		  k++;
		  
		}
  //  cout <<"メニューは" << k <<"種類です" << endl;
  return k;
}


int main(){
  int k=number();
  k=k-2;
  int i,j;
  writing_file.open(filename,std::ios::out);
  array<ticket,maxnum> stars;
   ifstream ifs("../ftp_files/tickets.info");
   
   /*std::ifstream ifs("tickets1.info");   //main内の省略したプログラムは、getlineで       一行ずつ構造体に入れることが出来ないか試したもの(とりあえず中断
   //std::string str;
   i=0;*/
   
    for(int i=0; i<k ;i++){
	  //while(getline(ifs,str)) {
	  
  ifs >> stars[i].number >> stars[i].nownum >> stars[i].orinum
		 >> stars[i].jyuppun >> stars[i].menu >> stars[i].price; 
  
  // i++;
   }
		
      for(i=0;i<k;i++){
		stars[i].sellnum= stars[i].orinum - stars[i].nownum;
		stars[i].percent= stars[i].sellnum/ stars[i].orinum;
		/*		stars[i].perorder=stars[i].number;
		stars[i].sellorder=stars[i].number;
		stars[i].jyuorder=stars[i].number; */
		/*	writing_file<<stars[i].number<<" " << stars[i].nownum <<" " << stars[i].orinum <<" " 
			  << stars[i].jyuppun<<" "  << stars[i].menu<<" "  << stars[i].price 
			  <<std::endl;*/
	  }
	  //割合の順位付け
	    for(i=0;i<k;i++){
	stars[i].perrank=1;
	for(j=0;j<k;j++){
	  if(stars[i].percent<stars[j].percent){
		stars[i].perrank++;
	  }
	}
  }
	    
		//売上数の順位付け
	    for(i=0;i<k;i++){
	stars[i].sellrank=1;
       
	for(j=0;j<k;j++){
	  if(stars[i].sellnum<stars[j].sellnum){
		stars[i].sellrank++;
	  }
	}
  }
		//直近十分の売上の勢いの順位付け
		for(i=0;i<k;i++){
	stars[i].jyurank=1;
	for(j=0;j<k;j++){
	  if(stars[i].jyuppun<stars[j].jyuppun){
		stars[i].jyurank++;
	  }
	}
  }
		//それぞれの一番と二番を探すプログラム（数字をいじれば簡単に可変)
	    int topper,topsell,topjyu,topper2,topsell2,topjyu2;
        int secper,secsell,secjyu;
		
		int h=0;
		int w=0;
		int z=0;
		for(i=0;i<k;i++){

		  //perrankの順位渡し
		  if(stars[i].perrank == 1){
			h=h+1;
				if(h >1){
			  topper2=topper;
			  } // hが１以上で例外処理(topが２つ) ここは未完成

			topper=i;
		  } else if(stars[i].perrank == 2){
			secper=i;
		  }

		  //sellrankの順位渡し
		  if(stars[i].sellrank == 1){
						w=w+1;
				if(w >1){
			  topsell2=topsell;
			  } // wが１以上で例外処理(topが２つ) ここは未完成

			topsell=i;
		  } else if(stars[i].sellrank == 2){
			secsell=i;
		  }
		  //jyurankの順位渡し
		  if(stars[i].jyurank == 1){
			z=z+1;
				if(z >1){
			  topjyu2=topjyu;
			  } // hが１以上で例外処理(topが２つ) ここは未完成

			topjyu=i;
		  } else if(stars[i].jyurank == 2){
			secjyu=i;
		  }
		}
		//何も売れていない場合
		int p=0;
  for(i=0;i<k;i++){
    if( stars[i].sellnum!=0){
      p=p+1;
    }
  }

  		//10ppunがまだの場合
		int ww=0;
  for(i=0;i<k;i++){
    if( stars[i].jyuppun!=0){
     ww=ww+1;
    }
  }

		int uu;
		  uu=0;
		writing_file<<"直ぐに売り切れる可能性が高いもの\n-------------------------------------------"<<std::endl;
		for(i=0;i<k;i++){
		  if( stars[i].jyuppun*5 >= stars[i].nownum){
		    writing_file <<stars[i].menu<<"は5分以内に売り切れる可能性が高いです"<<std::endl;
		    uu=1;
		  }else
		  if( stars[i].jyuppun*10 >= stars[i].nownum){
		    writing_file <<stars[i].menu<<"は10分以内に売り切れる可能性が高いです"<< std::endl;
		    uu=1;
		  }else if( stars[i].jyuppun*15 >= stars[i].nownum){
		    writing_file <<stars[i].menu<<"は15分以内に売り切れる可能性が高いです"<<std::endl;
		    uu=1;
		  }else if( stars[i].jyuppun*20 >= stars[i].nownum){
		    writing_file <<stars[i].menu<<"は20分以内に売り切れる可能性が高いです"<< std::endl;
		    uu=1;
		  }}

		if(uu==0){
		    	writing_file<<"20分以内に売り切れそうなものはありません"<<std::endl;
			uu=1;
		  }
		  writing_file<<"-------------------------------------------"<<std::endl;


		  writing_file<<"売れ行き情報\n"<<std::endl;
		  if(p==0){
		  	  writing_file<< "まだ何も売れていません"<<std::endl;}
		  else{
		if(w == 1){
		
		  writing_file<< "一番売れているのは" <<stars[topsell].menu<<"です"<<std::endl;}
		else /* if(w==2) */{   writing_file<< "一番売れているのは" <<stars[topsell].menu<<"と"<<stars[topsell2].menu<<"です"<<std::endl;}

		  
	if(h == 1){
			  
	  writing_file<< "1番売りきれるのが早いのは" <<stars[topper].menu<<"です"<<std::endl;}
	else /* if(h==2) */ {   writing_file<< "一番売りきれるのが早いのは" <<stars[topper].menu<<"と"<<stars[topper2].menu<<"です"<<std::endl;}

        	  if(ww==0){
		  	  writing_file<< "\n"<<std::endl;}
		  else{
	

	if(z ==1){
	  writing_file<<"今一番勢い良く売れているのは"<<stars[topjyu].menu<<"です"<<std::endl;}
	else /* if(z==2) */ {   writing_file<< "一番勢い良く売れているのは" <<stars[topjyu].menu<<"と"<<stars[topjyu2].menu<<"です"<<std::endl;}}}


	/*   for(int i=0; i<k ;i++){
	 if(stars[maxprinum].price<stars[i].price){
	   maxprinum=i;
	 }  
	 }
   
	 cout<<stars[maxprinum].price <<endl;//高いメニューの値段を表示  
		for(i=0;i<k;i++){
	 cout<<setprecision(2) <<stars[i].percent <<endl; //売れた割合を小数点第２位まで表示させた
	 } */
	writing_file.close();
		return 0;
   }


	

  
  









/*ticket t_tic[4]={{ 1, 199, 200, 0 ,"カレー", 310},
				 { 2, 200, 200, 0, "そば",250},
				 {3, 250, 250, 0,"味噌ラーメン",290},
				 {4, 250, 250, 0,"塩ラーメン",290}}; */

/* for(i=0;i<4;i++ ){
	cout<<"a; "  <<tic.menu(i) <<endl;
	cout<<" a; " <<tic.price(i) <<endl;
	cout<<" a; " <<tic.nownum(i) <<endl;
	}:*/

/*   for(const auto &tic : stars){
	 cout<<"a; "  <<tic.menu <<tic.price <<endl;
  } */
