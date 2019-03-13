#include <iostream>
#include "src/nn/NeuralNet.h"
#include "src/utils/DatasetCar.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <QLineSeries>
#include <QChartView>
#include <QChart>
#include <QApplication>
#include <QMainWindow>

using namespace QtCharts;


cv::Mat getImage(DatasetCar& dataset, int index) {
    cv::Mat image = imread(dataset[index].filename);
    cv::Mat imageNormalized;
    image.convertTo(imageNormalized, CV_32F, 1.0 / 255, 0);
    cv::resize(imageNormalized, imageNormalized, cv::Size(64, 64));
    return imageNormalized.reshape(1,1);
}

float getNormalY(DatasetCar& dataset, int index) {
    double y = (dataset[index].steeringAngle - dataset.minFeatureValue) /
            (dataset.maxFeatureValue - dataset.minFeatureValue);
    if (dataset.maxFeatureValue - dataset.minFeatureValue == 0) {
        cout << "Note, y is being set to 0" << endl;
        y = 0;
    }

    return (float)y;
}

cv::Mat getBinnedY(float y, int bins) {
    // Convert y to a vector dtype
    cv::Mat arrayOfArrays = (cv::Mat_<float>(1, 1) << y);
    vector<int> channels = {0};
    vector<int> histSize = {bins};
    vector<float> ranges = {0.f, 1.f};
    cv::Mat histogram;
    cv::calcHist(arrayOfArrays, channels, cv::Mat(), histogram, histSize,ranges);
    cv::transpose(histogram, histogram);
    return histogram;
}

cv::Mat getGuassianBinnedY(cv::Mat y, int smoothSize) {
    cv::Mat smoothedY;
    GaussianBlur(y, smoothedY, cv::Size(smoothSize, smoothSize), 0, 0, cv::BORDER_CONSTANT);
    return smoothedY;
}

void runNN() {
    // Load dataset
    DatasetCar dataset(10);
    dataset.readCsv(100, true);

    vector<DatasetCar> d = dataset.split(.01);
    DatasetCar trainDataset = d[0];
    DatasetCar validationDataset = d[1];

    // Create neural net
    NeuralNet nn = NeuralNet();
    nn.addLayer(64*64*3, 10, 0, "random");
    nn.addLayer(10, 60, 0, "random");
    nn.addLayer(60, 1, 0, "random");

    // Init the validation variables
    cv::Mat validationX;
    cv::Mat validationY;
    for (int i = 0; i < validationDataset.getSize(); i++) {
        validationX.push_back(getImage(validationDataset, i));
        validationY.push_back(getBinnedY(getNormalY(validationDataset, i), 60));
    }

    // Define epochs
    int epochs = 600;
    for (int epoch = 0; epoch < epochs; epoch++) {
        printf("Starting epoch %i\n", epoch);

        cv::Mat y;
        cv::Mat predY;

        int batchSize = 50;
        for (int i = 0; i < trainDataset.getSize(); i += batchSize) {
            cv::Mat batchX;
            cv::Mat batchY;
            printf("\tStarting batch %i\n", i);

            for (int imageLoc = i, slot = 0; imageLoc < i + batchSize and imageLoc < trainDataset.getSize();
                 imageLoc++, slot++) {
                batchX.push_back(getImage(trainDataset, imageLoc));
                batchY.push_back(getBinnedY(getNormalY(trainDataset, imageLoc), 60));
            }

            y.push_back(batchY);
            predY.push_back(nn.forward(batchX));

            nn.train(batchX, batchY, 30, batchSize);
        }
        // Eval the RMSE independent of batches
        nn.logBatchRMSE(predY, y);
        nn.logBatchRMSEValidation(nn.forward(validationX), validationY);

        printf("Epoch %i RMSE: %f Validation RMSE: %f\n", epoch, nn.rmse.back(), nn.rmseValidate.back());
    }
}

int main() {

//    connect(sender, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(Qbject*)));

    // Init Qt charting
    int arg =1;
    char *  arc = "s";
    QApplication a(arg, &arc, true);

    QChart* chart = new QChart();
    auto* series = new QLineSeries();
    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simple line chart example");

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(400, 300);
    window.show();
    a.exec();

    return 0;
}

