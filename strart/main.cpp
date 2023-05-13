#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Header.h"
#include <thread>






int main(int argc, const char * argv[]) {






    Mat original = imread(filename, IMREAD_GRAYSCALE);// загрузка изображения

    Mat sobel = original.clone();//генерация Собеля/Кенни
    imshow("original", original);

    createTrackbar("xorder","original",&xorder, 1000);
    createTrackbar("yorder", "original", &yorder, 1000);
    createTrackbar("layer 1","original",&layer_1, 255);
    createTrackbar("layer 2","original",&layer_2, 255);
    createTrackbar("layer 3","original",&layer_3, 255);

    while(true) {// регулировка уровней для Собеля/Кенни
        Canny(original, sobel, xorder, yorder);
        imshow("Sobel", sobel);
        int k = waitKey(1);

        if(k==27){ //выход из цикла при нажатии ESC
            break;
        }
    }
    destroyWindow("Sobel");

    vector<Nail> nails;
    Mat for_nails = original.clone();
    while(true) {
        nails = get_nails(sobel, original, 1, grid_item, layer_1, layer_2, layer_3); // генерация точек
        for_nails = 255;
        draw_nails(for_nails, nails);
        imshow("Nails", for_nails);
        int k = waitKey(1);

        if(k==27){ //выход из цикла при нажатии ESC
            break;
        }
    }
    destroyWindow("Nails");

    get_neighbours(nails); // получение соседних точек для гвоздей

    Nail current_nail = nails[100];
    Nail current_nail2 = nails[100];


    Nail best_neighbor(0,0);
    Nail best_neighbor2(0,0);


    double quality = 100;
    double neighbour_quality = 100;
    Mat canvas = original.clone();
    canvas = 255;
    Mat pure_canvas = canvas.clone();

    draw_nails(canvas, nails);
    
    vector<Point> route;
    while (true) {
        for(auto &item : current_nail.neigh){
            line(pure_canvas, current_nail.p, nails[item].p, 0);
            blur(pure_canvas, pure_canvas, Size(2,2));
            neighbour_quality = compareMat(pure_canvas, original);

            if(neighbour_quality <= quality){
                quality = neighbour_quality;
                best_neighbor = nails[item];
            }
            pure_canvas = canvas.clone();
        }
        route.push_back(current_nail.p);
        line(canvas, current_nail.p, best_neighbor.p, 0);

        pure_canvas = canvas.clone();


//        for(auto &item : current_nail2.neigh){
//            line(pure_canvas, current_nail2.p, nails[item].p, 0);
//            blur(pure_canvas, pure_canvas, Size(2,2));
//            neighbour_quality = compareMat(pure_canvas, original);
//
//            if(neighbour_quality <= quality){
//                quality = neighbour_quality;
//                best_neighbor2 = nails[item];
//            }
//            pure_canvas = canvas.clone();
//        }
//
//        line(canvas, current_nail2.p, best_neighbor2.p, 0);
//
//        pure_canvas = canvas.clone();
        current_nail = best_neighbor;
        current_nail2 = best_neighbor2;




        imshow("String Art", canvas);


        if(waitKey(1) == 27){
            break;
        }

    }
    
    getDrillingGcode(route, "/Users/gevorgov/Desktop/drilling.txt");





    return 0;
}

//
//#include <iostream>
//#include <fstream>
//#include <cmath>
//
//using namespace std;
//
//// Define a struct Point to hold x and y coordinates
//struct Point {
//    double x, y;
//};
//
//// Define a function to calculate the distance between two points
//double distance(Point p1, Point p2) {
//    double dx = p1.x - p2.x;
//    double dy = p1.y - p2.y;
//    return sqrt(dx*dx + dy*dy);
//}
//
//// Define a function to generate G code for circular interpolation and avoid hitting nails
//void generateGCode(Point points[], int numPoints, double radius, string filename) {
//    ofstream outFile(filename);
//
//    // Write the G code header
//    outFile << "G21\n";
//    outFile << "G90\n";
//    outFile << "G0 X" << points[0].x << " Y" << points[0].y << "\n";
//
//    // Loop through all the points and generate G code
//    for (int i = 0; i < numPoints; i++) {
//        // Calculate the offset point to avoid hitting nails
//        Point offsetPoint;
//        if (i < numPoints - 1 && distance(points[i], points[i+1]) < 2*radius) {
//            double dx = points[i+1].x - points[i].x;
//            double dy = points[i+1].y - points[i].y;
//            double angle = atan2(dy, dx);
//            offsetPoint.x = points[i].x + radius*cos(angle);
//            offsetPoint.y = points[i].y + radius*sin(angle);
//        }
//        else {
//            offsetPoint = points[i];
//        }
//
//        // Generate G code for circular interpolation
//        outFile << "G3 X" << offsetPoint.x << " Y" << offsetPoint.y << " I" << radius << " J" << 0.0 << "\n";
//    }
//
//    // Write the G code footer
//    outFile << "G0 X" << points[0].x << " Y" << points[0].y << "\n";
//    outFile << "M2\n";
//
//    outFile.close();
//}
//
//// Define the main function to test the generateGCode function
//int main() {
//    // Define an array of points and the radius of the thread
//    Point points[] = {{0, 0}, {10, 0}, {10, 10}, {0, 10}, {0, 0}};
//    int numPoints = 5;
//    double radius = 2.5;
//
//    // Generate G code and save it to a file
//    generateGCode(points, numPoints, radius, "/Users/gevorgov/Desktop/Study/strart/strart/output.gcode");
//
//    return 0;
//}






//
//
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <fstream>
//#include <cstdlib>
//#include <iostream>
//#include <iomanip>
//#include <cmath>
//#include "delaunator.hpp"
//using namespace cv;
//using namespace std;
//const int alpha_slider_max = 100;
//int alpha_slider;
//double alpha;
//double beta;
//Mat dst;
//
//int count1 = 2;
//int count2 = 4;
//int count3 = 6;
//int count4 = 8;
//int pr1 = 200;
//int pr2 = 150;
//int pr3 = 100;
//int pr4 = 150;
//Mat image;
//Mat img1;
//Mat img2;
//Mat img3;
//Mat img4;
//int Max=255;
//
//void triangles(Mat image,vector<double> &p,int predel1,int count_point1) {
//    for (int i = 0; i < image.rows; i++) {
//        for (int j = 0; j < image.cols; j++) {
//            if ((double)image.at<uchar>(i,j) <= predel1) {
//                    if (i % (count_point1 ) == 0 && j % (count_point1  ) == 0) {
//                       // int ent=rand()%1000;
//                       // if(ent>300){
////                        p.push_back(j+rand()%4);
////                        p.push_back(i+rand()%4);
//                        p.push_back(j);
//                        p.push_back(i);
//
//                       // }
//
//                    }
//            }
//        }
//    }
//};
//void print(Mat img, Mat image,int predel1, delaunator::Delaunator d1, Scalar delaunay_color) {
//
//    for (int i = 0; i < d1.triangles.size() - 2; i=i+3) {
////        int ai = (d1.coords[2 * d1.triangles[i]] + d1.coords[2 * d1.triangles[i + 1]]) / 2;
////        int aj = (d1.coords[2 * d1.triangles[i] + 1] + d1.coords[2 * d1.triangles[i + 1] + 1]) / 2;
////        Point a(d1.coords[2 * d1.triangles[i]], d1.coords[2 * d1.triangles[i] + 1]);
////        Point b(d1.coords[2 * d1.triangles[i + 1]], d1.coords[2 * d1.triangles[i + 1] + 1]);
////        if ((double)image.at<uchar>(aj,ai) <= predel1 ) {
////            line(img,a , b, Scalar(0, 0, 0), 1, cv::LINE_AA, 0);
////        }
////        else {
////            if (h == 0) {
////                h = 1;
////                i++;
////            }
////
////        }
////        h++;
////        if (h == 2) {
////            i++;
////            h = 0;
////        }
//        Point a(d1.coords[2 * d1.triangles[i]], d1.coords[2 * d1.triangles[i] + 1]);
//        Point b(d1.coords[2 * d1.triangles[i + 1]], d1.coords[2 * d1.triangles[i + 1] + 1]);
//        Point c(d1.coords[2 * d1.triangles[i + 2]], d1.coords[2 * d1.triangles[i + 2] + 1]);
//        int ai = (d1.coords[2 * d1.triangles[i]] + d1.coords[2 * d1.triangles[i + 1]]) / 2;
//        int bi = (d1.coords[2 * d1.triangles[i+1]] + d1.coords[2 * d1.triangles[i + 2]]) / 2;
//        int ci = (d1.coords[2 * d1.triangles[i+2]] + d1.coords[2 * d1.triangles[i]]) / 2;
//
//        int aj = (d1.coords[2 * d1.triangles[i] + 1] + d1.coords[2 * d1.triangles[i + 1] + 1]) / 2;
//        int bj = (d1.coords[2 * d1.triangles[i+1] + 1] + d1.coords[2 * d1.triangles[i + 2] + 1]) / 2;
//        int cj = (d1.coords[2 * d1.triangles[i+2] + 1] + d1.coords[2 * d1.triangles[i] + 1]) / 2;
//
//            if ((double)image.at<uchar>(aj,ai) <= predel1 && (double)image.at<uchar>(bj,bi) <= predel1 && (double)image.at<uchar>(cj,ci) <= predel1 ) {
//                circle(img, a, 1, Scalar(0,0,255));
//                circle(img, b, 1, Scalar(0,0,255));
//                circle(img, c, 1, Scalar(0,0,255));
//
//        line(img,a , b, Scalar(0, 0, 0), 1, cv::LINE_AA, 0);
//        line(img,b , c, Scalar(0, 0, 0), 1, cv::LINE_AA, 0);
//        line(img,c , a, Scalar(0, 0, 0), 1, cv::LINE_AA, 0);
//
//
//            }
//
//    }
//};
//void stringArt(){
//    Scalar delaunay_color(0, 0, 0);
//    img1 = Scalar(255, 255, 255);
//    img2 = 255;
//    img3 = 255;
//    img4 = 255;
//
//                       vector<double> p1;
//                       vector<double> p2;
//                       vector<double> p3;
//                       vector<double> p4;
//                       if (count1 >2)
//                           triangles(image, p1, pr1, count1);
//                       if (count2 > 2)
//                           triangles(image, p2, pr2, count2);
//                       if (count3 > 2)
//                           triangles(image, p3, pr3, count3);
//                       if (count4 > 2)
//                           triangles(image, p4, pr4, count4);
//
//                       if (p1.size() > 4) {
//                           delaunator::Delaunator d1(p1);
//                           print(img1, image, pr1, d1, delaunay_color);
//                       }
//                       if (p2.size() > 4) {
//                             delaunator::Delaunator d2(p2);
//                             print(img1, image, pr2, d2, delaunay_color);
//                           print(img2, image, pr2, d2, delaunay_color);
//
//                       }
//                       if (p3.size() > 4) {
//                             delaunator::Delaunator d3(p3);
//                             print(img1, image, pr3, d3, delaunay_color);
//                           print(img3, image, pr3, d3, delaunay_color);
//
//                       }
//                       if (p4.size() > 4) {
//                             delaunator::Delaunator d4(p4);
//                             print(img1, image, pr4, d4, delaunay_color);
//                           print(img4, image, pr4, d4, delaunay_color);
//
//                       }
//                    imwrite("//Users//gevorgov//Desktop//Natalie.jpg", img1);
//    imwrite("//Users//gevorgov//Desktop//Layer_1.jpg", img2);
//    imwrite("//Users//gevorgov//Desktop//Layer_2.jpg", img3);
//    imwrite("//Users//gevorgov//Desktop//Layer_3.jpg", img4);
//
//}
////static void button_click(int,void*)
////{
////    cout<<"Hello world\n";
////}
//static void on_trackbar( int, void* )
//{
//   alpha = (double) alpha_slider/alpha_slider_max ;
//   beta = ( 1.0 - alpha );
//   addWeighted( image, alpha, img1, beta, 0.0, dst);
//
//}
//static void on_trackbar1( int, void* )
//{
//    //stringArt();
//    //on_trackbar( alpha_slider, 0 );
//}
//static void on_trackbar2( int, void* )
//{
//   // stringArt();
//   // on_trackbar( alpha_slider, 0 );
//}
//static void on_trackbar3( int, void* )
//{
//    //stringArt();
//    //on_trackbar( alpha_slider, 0 );
//}
//static void on_trackbar4( int, void* )
//{
//    //stringArt();
//    //on_trackbar( alpha_slider, 0 );
//}
//
//int main()
//{
////    setlocale(LC_ALL,"RUSSIAN");
//    const char* filename="/Users/gevorgov/Desktop/Nat.jpg";
////    image = imread(filename,IMREAD_GRAYSCALE);
////    img1 = imread(filename,IMREAD_GRAYSCALE);
////    Scalar delaunay_color(0, 0, 0);
////    img1 = 255;
////       namedWindow("string art", WINDOW_AUTOSIZE); // Create Window
////       createTrackbar( "Pr1", "string art", &pr1, Max, on_trackbar1 );
////       on_trackbar1( pr1, 0 );
////    createTrackbar( "Pr2", "string art", &pr2, Max, on_trackbar2 );
////    on_trackbar1( pr2, 0 );
////    createTrackbar( "Pr3", "string art", &pr3, Max, on_trackbar3 );
////    on_trackbar1( pr3, 0 );
////    createTrackbar( "Pr4", "string art", &pr4, Max, on_trackbar4 );
////    on_trackbar1( pr4, 0 );
////    createTrackbar( "conect", "string art", &alpha_slider, alpha_slider_max, on_trackbar );
////    on_trackbar( alpha_slider, 0 );
////
////
////    waitKey(0);
//    while (true) {
//        Mat checkim;
//        checkim = imread("//Users//gevorgov//Desktop//test.jpg",IMREAD_GRAYSCALE);
//
//             imshow("check",checkim);
//         int check=waitKey(0);
//        if(check==27)
//            break;
//        if(check==103)
//        {
//                image = imread(filename,IMREAD_GRAYSCALE);
//                img1 = imread(filename,IMREAD_GRAYSCALE);
//            img2 = img1.clone();
//            img3 = img1.clone();
//            img4 = img1.clone();
//                Scalar delaunay_color(0, 0, 0);
//                img1 = 255;
//                   namedWindow("string art", WINDOW_AUTOSIZE); // Create Window
//                   createTrackbar( "Pr1", "string art", &pr1, Max, on_trackbar1 );
//                   on_trackbar1( pr1, 0 );
//                createTrackbar( "Pr2", "string art", &pr2, Max, on_trackbar2 );
//                on_trackbar1( pr2, 0 );
//                createTrackbar( "Pr3", "string art", &pr3, Max, on_trackbar3 );
//                on_trackbar1( pr3, 0 );
//                createTrackbar( "Pr4", "string art", &pr4, Max, on_trackbar4 );
//                on_trackbar1( pr4, 0 );
////                createTrackbar( "conect", "string art", &alpha_slider, alpha_slider_max, on_trackbar );
////                on_trackbar( alpha_slider, 0 );
//            stringArt();
//            imshow( "string art", img1 );
//        }
//        cout<<check;
//    }
//
//  return 0;
//}
//
