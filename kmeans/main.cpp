#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// static void help()
// {
//     cout << "\nThis program demonstrates kmeans clustering.\n"
//             "It generates an image with random points, then assigns a random number of cluster\n"
//             "centers and uses kmeans to move those cluster centers to their representitive location\n"
//             "Call\n"
//             "./kmeans\n" << endl;
// }

int main( int /*argc*/, char** /*argv*/ )
{
    const int MAX_CLUSTERS = 5;
    Scalar colorTab[] =     //因为最多只有5类，所以最多也就给5个颜色
    {
        Scalar(0, 0, 255),
        Scalar(0,255,0),
        Scalar(255,100,100),
        Scalar(255,0,255),
        Scalar(0,255,255)
    };

    Mat img(500, 500, CV_8UC3);
    RNG rng(345); //随机数产生器,345为随机数种子

    while(true)
    {
        int clusterCount = rng.uniform(2, MAX_CLUSTERS+1);//uniform 指定随机数范围
        int sampleCount = rng.uniform(1, 1001);
        Mat points(sampleCount, 1, CV_32FC2), labels;   //产生的样本数，实际上为2通道的列向量，元素类型为Point2f

        clusterCount = MIN(clusterCount, sampleCount); //防止sample比cluster还少，虽然这种情况出现的概率很小
        Mat centers(clusterCount, 1, points.type());    //用来存储聚类后的中心点

        /* generate random sample from multigaussian distribution */
        for(int k = 0; k < clusterCount; k++ ) //产生随机数
        {
            Point center;
            center.x = rng.uniform(0, img.cols);
            center.y = rng.uniform(0, img.rows);
            Mat pointChunk = points.rowRange(k*sampleCount/clusterCount,
                                             k == clusterCount - 1 ? sampleCount :
                                             (k+1)*sampleCount/clusterCount);   //最后一个类的样本数不一定是平分的，(取整的问题)
                                                                                //剩下的一份都给最后一类
            //每一类都是同样的方差，只是均值不同而已
            rng.fill(pointChunk, CV_RAND_NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
        }
        
        kmeans(points, clusterCount, labels,
               TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
               3, KMEANS_PP_CENTERS, centers);  //聚类3次，取结果最好的那次，聚类的初始化采用PP特定的随机算法。

        img = Scalar::all(0);

        for(int i = 0; i < sampleCount; i++ )
        {
            int clusterIdx = labels.at<int>(i);
            Point ipt = points.at<Point2f>(i);
            circle( img, ipt, 8, colorTab[clusterIdx], CV_FILLED, CV_AA );
        }

        imshow("clusters", img);

        char key = (char)waitKey();     //无限等待
        if( key == 27 || key == 'q' || key == 'Q' ) // 'ESC'
            break;
    }

    return 0;
}
