//
//  Header.h
//  strart
//
//  Created by Артем Геворгов on 13.11.2022.
//

#ifndef Header_h
#define Header_h
#pragma once
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <set>
#include <iostream>

using namespace::std;
using namespace::cv;
static const int black = 0;
static const int limit = 5;
static const int grid_item = 10; // минимальное расстояние между точками
static const int neighbours_radius = grid_item * 4;
const string filename="/Users/gevorgov/Desktop/Monro.jpg";
const int depth = -8;

static  int xorder = 1;
static  int yorder = 1;
static  int layer_1 = 1;
static  int layer_2 = 1;
static  int layer_3 = 1;


struct Nail{
    Point p; // координаты
    int sch=0; // счетчик для ограничения для каждого гвоздя
    vector<int> neigh; // вектор с индексами соседними точками
    Nail(int x,int y); // конструктор
};
vector<Nail> get_nails(Mat sobel, Mat original, int predel1,int grid_size, int layer_1, int layer_2, int layer_3);
void get_neighbours(vector<Nail> &nails);
int get_density(double x);
void getDrillingGcode(vector<Point> route, string filename);
double compareMat(Mat img1,Mat img2);
double compareMatAVG(Mat img1,Mat img2);
void draw_nails(Mat canvas, vector<Nail> nails);
#endif /* Header_h */
