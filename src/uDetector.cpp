#include "yoloDetector.h"
#include <fstream>
#include <iostream>
#include <time.h> 
#include <string>

using namespace std;
using namespace yolo;

std::string replace_all(std::string& str,const std::string old_value,const std::string new_value)
{
	std::string str_new = str;
	while(true)
	{
		std::string::size_type pos(0);
		if((pos=str_new.find(old_value))!=string::npos)
			str_new.replace(pos,old_value.length(),new_value);
		else break;
	}
	return str_new;
}

int main(int argc, char* argv[])
{
	if (argc != 7)
	{
		cerr << "usage: ./uDetect cfgfile weightfile thresh gpuNo imglist outname" << endl;
		return -1;
	}
	yoloDetector detector(argv[1], argv[2], atof(argv[3]), atoi(argv[4]));

	ifstream inlist;
	inlist.open(argv[5]);
	if (!inlist.is_open())
	{
		cerr << "fail to open file list" << endl;
		return -2;
	}
	ofstream outfile;
	outfile.open(argv[6]);
	if (!outfile.is_open())
	{
		cerr << "fail to open output file" << endl;
		return -2;
	}
	time_t start, finish, total=0;

	while (!inlist.eof())
	{
		string img_name;
		inlist >> img_name;
		cout << img_name << endl;
		cv::Mat img = cv::imread(img_name);
		if (img.empty())
		{
			cerr << "fail to open image:" << img_name << endl;
			continue;
		}
		vector<objectNode> objlist;

		int j = 0;
		start = clock();
		detector.predict(img, objlist);
		//finish = clock();
		//cout << img_name  << "This Picture Time=" << (finish - start)/1000 << "ms" << endl;
		//total = total + (finish - start);

		/*outfile << img_name << " " << img.cols << " " << img.rows << " " << objlist.size() << endl;
		for (int i = 0; i < objlist.size(); i++)
		{
			objectNode node = objlist.at(i);
			outfile << node._rt.x << " " << node._rt.y << " " << node._rt.width << " " << node._rt.height << " " << node._class << endl;
			//outfile << node._rt.x << " " << node._rt.y << " " << node._rt.width << " " << node._rt.height << " " << node._class << endl;
		}*/
		

		string txt_name = replace_all(img_name, ".jpg",".txt");
		ofstream output_result(txt_name.c_str());
		if(!output_result)return 0;
		for (int i = 0; i < objlist.size(); i++)
		{	
			//if(det_out[i].name == clsType )
			//{
			objectNode node = objlist.at(i);
				//cv::rectangle(im, det_out[i].rect, cv::Scalar(0, 255, 0), 2);
				//std::cout << det_out[i].name << ": " << det_out[i].score*100 << "%%, ";
				//std::cout << det_out[i].rect.x << " " << det_out[i].rect.y << " " <<det_out[i].rect.width << " " << det_out[i].rect.height << std::endl;
				// cv::imshow("detection", im);
				// cv::waitKey(0);		
				//detboxes.push_back(cv::Rect(det_out[i].rect.x ,det_out[i].rect.y ,det_out[i].rect.width ,det_out[i].rect.height));
				//save_label
				// ofstream output_result(label_name);
				// if(!output_result)return 0;
				//output_result <<"1" << " "<<det_out[i].rect.x << " " << det_out[i].rect.y  << " "<< det_out[i].rect.width  << " "<< det_out[i].rect.height <<endl;
			output_result <<node._class  << " "<< node._rt.x << " " <<  node._rt.y   << " "<< node._rt.x + node._rt.width  << " "<< node._rt.y + node._rt.height <<endl;
				
			
			//}
			
		}
		output_result.close();


	}
	cout << "Total Time=" << total/1000 << "ms"<<endl;
	inlist.close();
	outfile.close();

	return 0;
}
