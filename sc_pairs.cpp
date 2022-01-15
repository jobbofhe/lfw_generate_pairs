
/*****************************************************************************
* Function	: 生成paris.txt. 
* Date		: 2018.03.02
* Author	: jobbofhe
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>  
#include <sstream>
#include <string> 
#include <string.h> 
#include <stdlib.h> 
#include <time.h> 
#include <algorithm>
#include<list>  
#include <typeinfo>

#include<dirent.h>   // /usr/include/dirent.h

using namespace std;



#define 	NO_MATCH	0		//不匹配对
#define 	MATCH 		1		//匹配对


//保存一个人名  他的图片随机取两张的组合
typedef struct _ONE_PEOPLE_COM
{
	string sName;
	vector<vector<int> > vOnePeoCom;	//eg: 10对: vOnePeoCom.size()  vOnePeoCom[0][0]  (1, 4) : 第一对： 1， 4
}ONE_PEOPLE_COM;


//保存 一个人名 人名目目录下的图片数量
typedef struct  _IMGS_INFO
{
	int iNum;
	string sName;
	
}IMGS_INFO;

list<int> sc_get_imgs_serial(int imgsNum, int iRetNum);		// 某个人的图像数量， 要获取几张随机图像


vector<string>& sc_combination (vector<string>& res, const size_t& choose, string& total, const size_t& pos);
vector<vector<int> > sc_combination_result(const size_t choose, int iTotal);
vector<int> sc_get_rand_norepeat(int iNum);
int sc_get_all_files(string directory, vector<string>& files);
vector<IMGS_INFO> sc_get_imgs_info(const string Path);




//排列组合的结果
vector<string>& sc_combination (vector<string>& res, const size_t& choose, string& total, const size_t& pos) 
{
	//pos 起始位置
	if (choose > total.size() - pos)  // 所选个数 大于总数
	{
 		return res;
	}

	for (size_t i = pos; i != total.size(); ++i) 
	{
		total[i] = '0';		//每一位置零
	}

	if (choose == 0 || pos == total.size())  //不取，或者取全部
	{
		res.push_back (total);
		return res;
	}

	total[pos] = '1';
	sc_combination (res, choose - 1, total, pos + 1);
	total[pos] = '0';
	sc_combination (res, choose, total, pos + 1);

	return res;
}


//传入文件总数 iTotal, 从中取choose 个的所有组合， 返回所有结果
vector<vector<int> > sc_combination_result(const size_t choose, int iTotal)
{
	vector<string> res;

	vector<int> vSerial;			//所有组合
	vector<vector<int> > vSubSer;	//choose个一组 分成M组

	string total (iTotal, '0');	//拷贝6个'0'到 total

	sc_combination (res, choose, total, 0);
	for (size_t i = 0; i != res.size(); ++i) 
	{
		for (size_t j = 0; j != total.size(); ++j) 
		{
			if (res[i][j] == '1') 
			{
				//cout << j + 1 << ' ';
				vSerial.push_back(j+1);
			}
		}
	}

	vSubSer.resize(vSerial.size() / choose, vector<int>(choose));
	
	for (int i = 0;  i < vSerial.size(); i++)
	{
		vSubSer[i / choose][i % choose] = vSerial[i]; // 一维数组转为二维数组
	}

	return vSubSer;
}


/*
//随机获取两张图像 
// 未使用 
list<int> sc_get_imgs_serial(int imgsNum, int iRetNum)		// 某个人的图像数量， 要获取几张随机图像
{

	srand((unsigned)time(NULL));  
    list<int> listSerials;			//定义链表，保存生成的随机数  
    int begin = 1;
    int end = imgsNum;					//数字范围  
    int sum = iRetNum;					//随机数个数  

    if ( (end < 0) || (begin < 0) || (begin > end) || (sum > end) )	//起始范围必须大于0,且随机数个数小于等于最大数字范围  
    {  
        cout << "eror: parameter range error" <<  endl;  
        return  listSerials;  
    }  
    else  
    {  
        while (listSerials.size() < sum)  
        {  
            listSerials.push_back( rand() % (end - begin + 1) + begin );  
            listSerials.sort();			//排序  
            listSerials.unique();		//去除相邻的重复随机数中的第一个  
        }  
    }  

    return listSerials;
}
*/

//随机生成范围 iNum 内的iNum个随机数
vector<int> sc_get_rand_norepeat(int iNum)
{
	vector<int> temp;

	if( iNum < 0)
	{
		cout << "error" << endl;
		return temp;
	}

	for (int i = 0; i < iNum; i++)
	{
	 	temp.push_back(i + 1);
	}
	random_shuffle(temp.begin(), temp.end());

	return temp;
}

/*
    Para        : std::string directory, std::vector<std::string>& files    ：图片一级路径，所有图片名称
    Return      : string                          : 返回 对应iNum 的图片名称
    Function    : 遍历目录，获取目录下的所有文件【一级目录下的问及目录， 或者二级目录下的文件】
*/
int sc_get_all_files(string directory, vector<string>& files)
{

	DIR* dir = opendir(directory.c_str()); 		//打开目录   DIR-->类似目录句柄的东西 

	if ( dir == NULL )
	{
		cout << directory << " is not a directory or not exist!" << endl;
	   	return -1;
	}

	struct dirent* d_ent = NULL;       	//dirent-->会存储文件的各种属性
	char dot[3] = ".";                	//linux每个路径下面都有一个 .  和 ..  要把这两个都去掉
	char dotdot[6] = "..";
	int cnt = 0;

	//readdir 线程非安全
	while ( (d_ent = readdir(dir)) != NULL )    //一行一行的读目录下的东西,这个东西的属性放到dirent的变量中
	{

		if ( (strcmp(d_ent->d_name, dot) != 0) && (strcmp(d_ent->d_name, dotdot) != 0) )   //忽略 . 和 ..
		{
		
	 		//if ( (d_ent->d_type & DT_DIR) ) //d_type可以看到当前的东西的类型,DT_DIR代表当前得到的是目录
 			files.push_back(d_ent->d_name);
 			cnt++;
 		}
	}
	//closedir(directory.c_str());

	return cnt;
}

//获取图片信息
vector<IMGS_INFO> sc_get_imgs_info(const string Path)
{
	//遍历图片目录，取得每一个子目录的名称
	const string personPath = Path;   	//保存每个人脸数据的路径
	cout << personPath << endl;
	
	vector<string> dirPers;					//每个人的名字，也是文件夹的名称
	vector<string> imgs;					//每个人的图像
	vector<IMGS_INFO> vImgsInfo;
	IMGS_INFO tImg = { 0 };


	string subPath = "";	//每个人的路径
	int imgsCnt = 0;		//同一个人图片数量
	char chCnt[4] = "";		

	string buf = "";

	std::ofstream output_people;
	output_people.open("people.txt");

	sc_get_all_files(personPath, dirPers);			//取得目录下所有人的对应目录

	for( int i = 0;  i < dirPers.size(); i++)
	{
		subPath = personPath + "/" + dirPers[i];
		imgsCnt = sc_get_all_files(subPath, imgs);	//获取每个人的图片数量
		
		tImg.sName = dirPers[i];
		tImg.iNum = imgsCnt;
		vImgsInfo.push_back(tImg);

		sprintf(chCnt, "%d", imgsCnt);
		string str(chCnt);
		buf = dirPers[i] + "\t\t" + str + "\n";		// 人名		图片数量

		cout << buf << endl;
		
		output_people << buf;	//对应的人名+图片数量写入txt

		imgs.clear();
	}

	return vImgsInfo;

}


struct lfw_pair_t
{
    std::string filename1;
    //dlib::rectangle face1;
    int face1;

    std::string filename2;
   // dlib::rectangle face2;
    int face2;

    bool are_same_person;
};

struct lfw_pair_t2
{
    std::string filename1;
    //dlib::rectangle face1;
    string facePath1;

    std::string filename2;
   // dlib::rectangle face2;
    string facePath2;

    bool are_same_person;
};

struct lfw_pair
{
    std::string filename1;
   // dlib::rectangle face1;

    std::string filename2;
   	//dlib::rectangle face2;

    bool are_same_person;
};


int g_flg = -1;

vector<lfw_pair_t> sc_get_img_attr(lfw_pair_t tOneAttr)
{

#if 1

    ifstream iFile("./sc_pairs.txt");
    vector<lfw_pair_t> vAllAttr;

    if( !iFile )
    {
        cout << "file sc_pairs.txt is not exist!" << endl;
        return vAllAttr;
    }

    char buf[200];
    int i,j;

    g_flg = MATCH;

    for (i = 0; !iFile.eof(); i++)
    {
        iFile.getline(buf, 200);

        if( strcmp("#NO_MATCH", buf) == 0)
        {
        	g_flg = NO_MATCH;
        	continue;
        }

        if( g_flg == MATCH )
        {
        	istringstream istr(buf);

	       	istr >> tOneAttr.filename1;
	       	tOneAttr.filename2 = tOneAttr.filename1;
	      	istr >> tOneAttr.face1;
	      	istr >> tOneAttr.face2;
	      	tOneAttr.are_same_person = 1;
        }

        if( g_flg == NO_MATCH )
        {
        	istringstream istr(buf);

	       	istr >> tOneAttr.filename1;
	      	istr >> tOneAttr.face1;
	      	istr >> tOneAttr.filename2;
	      	istr >> tOneAttr.face2;
	      	tOneAttr.are_same_person = 0;
        }
        vAllAttr.push_back(tOneAttr);

       	//cout << i << "  " << tOneAttr.filename1 << " " << tOneAttr.filename2 << " "  << tOneAttr.face1 << " " << tOneAttr.face2;
       	cout << endl;
    }

    iFile.close();


#endif

    return vAllAttr;
}


string sc_get_img_name_by_img_num(string path, int imgNum)
{
	vector<string> files;

	sc_get_all_files(path, files);

	string tmp;
	stringstream ss;
	ss << imgNum;
	ss >> tmp;

	string reName;

	for(int j =0; j < files.size(); j++)
	{
		int re = files[j].find(tmp);  //hsq 大于10  出现bug

		if( re != -1 ) 		//找到第 pairs_tmp[i].filename1 对中 第 pairs_tmp[i].face1  张图片
		{
			reName = files[j];
		}
	    
	}
	files.clear();
	return reName;
}


vector<lfw_pair_t2> get_lfw_pairs_info(const string path)
{
    //using namespace dlib;
    std::vector<lfw_pair_t2> pairs_tmp2; 
    std::vector<lfw_pair_t> pairs_tmp; 


    lfw_pair_t tOneAttr;
    lfw_pair_t2 tOneAttr_s;
    vector<string> files;

    //读取sc_paris.txt 将人名以及图片赋值给 结构体 pairs_tmp
    pairs_tmp = sc_get_img_attr(tOneAttr);

    const string personPath = path;    //保存每个人脸数据的路径

    for(int i = 0; i < pairs_tmp.size(); i++)		//所有对数
    {
        string path1 = personPath + "/" + pairs_tmp[i].filename1;	
        string path2 = personPath + "/" + pairs_tmp[i].filename2;

        tOneAttr_s.are_same_person = pairs_tmp[i].are_same_person;

        cout << tOneAttr_s.filename1 << " --- " << tOneAttr_s.filename2<< endl;

        string imgName1 = sc_get_img_name_by_img_num(path1, pairs_tmp[i].face1);
        string imgName2 = sc_get_img_name_by_img_num(path2, pairs_tmp[i].face2);

        //取得每一对的图片路径+图片名
        
        //图片名称
        tOneAttr_s.filename1 = pairs_tmp[i].filename1 + "/" + imgName1;
        tOneAttr_s.filename2 = pairs_tmp[i].filename2 + "/" + imgName2;

        tOneAttr_s.facePath1 = path1 + "/" + imgName1;
        tOneAttr_s.facePath2 = path2 + "/" + imgName2;
		
        //cout << tOneAttr_s.facePath1 << "  " << tOneAttr_s.facePath2  << " " << tOneAttr_s.are_same_person << endl;

        pairs_tmp2.push_back(tOneAttr_s);

        cout <<endl <<endl;

    }

    return pairs_tmp2;
}


/*
vector<lfw_pair>get_lfw_pairs(vector<lfw_pair_t2> vParisInfo)
{
	//加载图片，获取
    //load_image(img, imagePath);
    //std::vector<rectangle> dets = detector(img);  //hog检测.

    vector<lfw_pair> pairs;
    matrix<rgb_pixel> img1;
    matrix<rgb_pixel> img2;

    vector<rectangle> dets;
    vector<rectangle> dets2;


    lfw_pair tPair;

	for ( int i = 0; i < vParisInfo.size(); i++ )
	{
		tPair.filename1 = vParisInfo[i].filename1;
		tPair.filename2 = vParisInfo[i].filename2;

		load_image(img1. vParisInfo[i].facePath1);
		dets1 = detector(img1);

		tPair.face1 = dets1;

		load_image(img2. vParisInfo[i].facePath2);
		dets2 = detector(img2);
		tPair.face2 = dets2;

		pairs.push_back(tPair);

	}

	return pairs;

}
*/


int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "run : ./sc_pairs ./lfw_mini" <<endl;
		return -1;
	}
	const string path = argv[1];

#if 0
/*
	lfw_pair_t tmp;
	vector<lfw_pair_t> vAllAttr;
	vAllAttr = sc_get_img_attr(tmp);
	cout << "--------------------------------------------------------" << endl;
	cout << "size:" << vAllAttr.size() << endl;
*/
/*
	for (int i = 0; i < vAllAttr.size() - 1; i++ )
	{
		cout << vAllAttr[i].filename1 << "\t" << vAllAttr[i].filename2 << "\t"  << vAllAttr[i].face1 << "\t" << vAllAttr[i].face2 << "\t" << vAllAttr[i].are_same_person << endl;
	}
*/
	vector<lfw_pair_t2> pairsInfo;
	//获取 sc_pairs.txt 的信息，存成对应 图片名  图片名  图片路径+图片名  图片路径+图片名
	pairsInfo = get_lfw_pairs_info(path);

	for( int i = 0; i < pairsInfo.size(); i++)
	{
	    cout <<pairsInfo[i].filename1 << "\t" << pairsInfo[i].facePath1 << "\t" << pairsInfo[i].facePath2  << "\t" << pairsInfo[i].are_same_person << endl;
		
	}
	//将pairsInfo 转换成 图片名 图片名  rectangle(图片处理只有的数据类型 dlib )  rectangle(图片处理只有的数据类型 dlib )
	get_lfw_pairs(pairsInfo);
#endif




//---------------------------------------------------------------------------------------------
	//生成sc_pairs.txt
#if 1
	vector<IMGS_INFO> vImgs;		//人名 	数量
	vector<ONE_PEOPLE_COM> vAllCom;	//人名	组合个数

	vector<int> cmpSer;

	std::ofstream output_paris;
	output_paris.open("sc_pairs.txt", ios::app);

	string bufParis = "";
	vector<string> sCnt;
	char chCnt[4] = { 0 };

	ONE_PEOPLE_COM tOneCom;
	ONE_PEOPLE_COM tOneCom_10;

	//遍历图片目录，取得每一个子目录的名称， 以及子目录图片数量 :  
	//example:
	//		job  	5 
	//		jack	6 	
	vImgs = sc_get_imgs_info(path);

	cout << " sub dir num : " << vImgs.size() << endl;			//人数


	//每个人的图片 2个一组 分成M组  例如：job 6张图片，两两组合形成 6！/(2!4!) 对  
	for (int i = 0;  i < vImgs.size(); i++)
	{
		tOneCom.sName = vImgs[i].sName;
		tOneCom_10.sName = vImgs[i].sName;

		cout << vImgs[i].sName << "have imgs:  " << vImgs[i].iNum << endl; 

		if(vImgs[i].iNum < 2)
		{
			continue;  //如果某个人目录下只要一张图片 则不进行自己的匹配
		}

		tOneCom.vOnePeoCom = sc_combination_result(2, vImgs[i].iNum);	//6 中 取2的不同组合 ,一个人的所有组合

		if(tOneCom.vOnePeoCom.size() > 30)  		//保存一个人的前30对组合
		{	
			for (int j = 0; j < 30; j++)
			{
				tOneCom_10.vOnePeoCom.push_back(tOneCom.vOnePeoCom[j]);
			}
			vAllCom.push_back(tOneCom_10);
			tOneCom_10.vOnePeoCom.clear();
		}
		else
		{
			for(int k = 0; k < tOneCom.vOnePeoCom.size(); k++)
			{
				tOneCom_10.vOnePeoCom.push_back(tOneCom.vOnePeoCom[k]);
			}

			vAllCom.push_back(tOneCom_10);
			tOneCom_10.vOnePeoCom.clear();

		}
		tOneCom.vOnePeoCom.clear();
		cout << endl;
	}


	//------------------------------------------------------------------------------------
	//匹配的 所有人组合
	printf("------------------------------------< match combination >-----------------------------------\n");
	for (int i = 0; i < vAllCom.size(); i++)							//所有人
	{
		for (int j = 0; j < vAllCom[i].vOnePeoCom.size(); j++)			//一个人所有组合数
		{
			for( int k = 0; k < vAllCom[i].vOnePeoCom[j].size(); k++)  	// 挑出的匹配对
			{
				//cout << vAllCom[i].vOnePeoCom[j][k] << " ";

				sprintf(chCnt, "%d", vAllCom[i].vOnePeoCom[j][k]);
				string str(chCnt);
				sCnt.push_back(str);

			}
			bufParis = vAllCom[i].sName  + "\t" + sCnt[0] + "\t" + vAllCom[i].sName + "\t" + sCnt[1] + "\n";

			//写入文件
			output_paris << bufParis;
			cout << bufParis;
			sCnt.clear();
		}
		cout << endl;
	}
	output_paris.close();

	output_paris.open("sc_pairs.txt", ios::app);

	printf("------------------------------------< no match combination >-----------------------------------\n");
	//不匹配的所有对数
	vector<vector<int> > vDifPeoCom;		//不匹配对
	vector<int>  iDifSer;					//一对的序号
	cout << "3333333 vImgs.size() :  " << vImgs.size() << endl;

	vDifPeoCom = sc_combination_result( 2, vImgs.size() );		//所有不匹配对数


	string bufNoMatch = "";
	vector<string> sNoCnt;
	char chNoCnt[4] = { 0 };

/* only test 
	for(int i = 0; i < vDifPeoCom.size(); i++)
	{
		for( int j =0 ; j < vDifPeoCom[i].size(); j++)
		{
			cout << vDifPeoCom[i][j] << "   " ;
		}
		cout << endl;
	}
*/
	string str1 = "#NO_MATCH";
	string str2 = "\n";

	string sSeparate = str1 + "\n";
	output_paris << sSeparate;			 			//txt文档中插入分隔标识

	for(int i = 0; i < vDifPeoCom.size(); i++)		//每一个不匹配的对 随机各个抽取一张图片  15  2 6 
	{
		for ( int j = 0; j < vDifPeoCom[i].size(); j++ )    //2
		{
			iDifSer = sc_get_rand_norepeat(vImgs[vDifPeoCom[i][j] - 1].iNum) ;

			//cout << vImgs[vDifPeoCom[i][j]-1].sName << " \t\t";
			//cout << iDifSer[j] << "\t\t";

			bufNoMatch.append(vImgs[vDifPeoCom[i][j]-1].sName);
			bufNoMatch.append("\t");

			sprintf(chNoCnt, "%d", iDifSer[j]);
			string str(chNoCnt);
			sNoCnt.push_back(str);
			bufNoMatch.append(sNoCnt[j]);
			bufNoMatch.append("\t");
		}
		
		cout << bufNoMatch ;
		bufNoMatch.append("\n");

		//写文件
		output_paris << bufNoMatch;

		bufNoMatch = "";

		sNoCnt.clear();
		iDifSer.clear();

		cout << endl;
	}
	output_paris.close();

#endif


	return 0;
}


