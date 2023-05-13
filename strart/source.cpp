//
//  source.cpp
//  strart
//
//  Created by Артем Геворгов on 13.11.2022.
//
#include "Header.h"


Nail::Nail(int x,int y){
    p=Point(x,y);
}


vector<Nail> get_nails(Mat sobel, Mat original, int predel1, int grid_size, int layer_1, int layer_2, int layer_3) {
    bool br=false;
    vector<Nail> nails;
    
    int l1 = get_density(layer_1);
    int l2 = get_density(layer_2);
    int l3 = get_density(layer_3);
    
    
    for (int i = 0; i < sobel.cols; i++) {
        for (int j = 0; j < sobel.rows; j++) {
            
            if( i % l1 == 0 && j % l1 == 0 && (int)original.at<uchar>(j,i) <= layer_1 ){
                nails.push_back(Nail(i,j));
            }
            
            if( i % l2 == 0 && j % l2 == 0 && (int)original.at<uchar>(j,i) <= layer_2  && (int)original.at<uchar>(j,i) > layer_1 ){
                nails.push_back(Nail(i,j));
            }
            
            if(i % l3 == 0 && j % l3 == 0 && (int)original.at<uchar>(j,i) <= layer_3   && (int)original.at<uchar>(j,i) > layer_2 ){
                nails.push_back(Nail(i,j));
            }

            
            if (i % (grid_size) == 0 && j % (grid_size) == 0) {
                for(int c = i; c < i + grid_size; c++){
                    for(int k = j; k < j + grid_size; k++){
                        if ((double)sobel.at<uchar>(k, c) >= predel1) {
                            nails.push_back(Nail(c, k));
                            br=true;
                            break;
                        }
                    }
                    if(br){
                        break;
                    }
                }
                br=false;
            }
        }
    }
    return nails;
};

void get_neighbours(vector<Nail> &nails){
    for(int i=0; i<nails.size();++i){
        for(int j=0;j<nails.size();j++){
            if(i!=j){
                if( abs( nails[i].p.x - nails[j].p.x ) < limit && abs( nails[i].p.y - nails[j].p.y ) < limit ){
                    nails.erase(nails.begin() + j);
                }
            }
        }
    }
    for(int i=0; i<nails.size();++i){
        for(int j=0;j<nails.size();j++){
            if( abs( nails[i].p.x - nails[j].p.x ) <= neighbours_radius && abs( nails[i].p.y - nails[j].p.y ) <= neighbours_radius  && (nails[i].p != nails[j].p ) ){
                nails[i].neigh.push_back(j);
            }
        }
    }
};

int get_density(double x){
    int res = (log(x / 24) * 5) + 10;
    return res < grid_item ? grid_item : res;
}

void getDrillingGcode(vector<Point> route, string filename){
    ofstream fout(filename);
    for(auto &item: route){
        fout<< 'X' << item.x << " Y" << item.y << " \nZ" << depth << endl;
    }
    fout.close();
}

double compareMat(Mat img1,Mat img2)
{
//    Mat test=img1.clone();
    double count=0;
    for(int i=0;i<img1.cols;i++)
    {
        for(int j=0;j<img1.rows;j++)
        {
            count += abs( img1.at<uchar>(j, i) - img2.at<uchar>(j, i) );
//            test.at<uchar>( j, i )=abs( img1.at<uchar>(j,i)-img2.at<uchar>( j, i ) );
        }
    }
    count /= ( img1.rows * img1.cols * 255 );
    return count;
};

double compareMatAVG(Mat img1,Mat img2){
    double count = 0;
    for(int i=0;i<img1.cols;i++)
    {
        for(int j=0;j<img1.rows;j++)
        {
            count += sqrt( ( img1.at<uchar>(j, i) - img2.at<uchar>(j, i)) * (img1.at<uchar>(j, i) - img2.at<uchar>(j, i) ));
        }
    }
    count /= ( img1.rows * img1.cols );
    return count;
}

void draw_nails(Mat canvas, vector<Nail> nails){
    for (int i=0; i<nails.size(); i++) { // отрисовка точек
        circle(canvas, nails[i].p, 1, 0);
    }
}


