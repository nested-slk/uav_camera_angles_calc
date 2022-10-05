#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::vector<float> camera_coord {0,0};
    std::vector<float> target_coord {0,0};
    std::vector<float> target_coord_in_uav {0,0};
    std::vector<float> uav_coord {0,0,0,0}; //x,y,z,angle

    std::vector<float> v {0,0};
    std::vector<float> v_normalize {0,0};
    std::vector<float> w {0,0};
    std::vector<float> w_normalize {0,0};

    std::vector<float> a {1,0}; // absolytely vector xy
    std::vector<float> a_normalize {1,0};
    std::vector<float> az {0,1}; // absolytely vector z
    std::vector<float> az_normalize {0,1};
    bool camera_xy_change_dir = false;
    float angle_target_to_north;
    int scale_x = 25000;
    int scale_y = 25000;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calcBttn_clicked();

private:
    Ui::MainWindow *ui;
    float calcAngleVectors(std::vector<float>& _v_normalize, std::vector<float>& _a_normalize, bool _dir);

    void calcVectorSub(std::vector<float>& _out_vector, std::vector<float>& _a_vector, std::vector<float>& _b_vector);
    void calcVectorSub(std::vector<float>& _out_vector, float _x1, float _y1, float _x2, float _y2);
    void calcVectorSub(std::vector<float>& _out_vector, std::vector<float>& _b_vector);

    void calcZeroVector(std::vector<float>& _uav, std::vector<float>& _a);
    void calcZeroVector(std::vector<float>& _out, float _x, float _y);

    void normalizeVector(std::vector<float>& _out, std::vector<float>& _in);
    void normalizeVector(std::vector<float>& _out, float _x, float _y);

    float calcVectorLenght(std::vector<float>& _v);
    float calcVectorLenght(float _a, float _b);

     QGraphicsScene *scene;
     QGraphicsScene *sceneZ;
     QGraphicsLineItem *camera_x_line;
     QGraphicsEllipseItem *ellipse;
     QGraphicsRectItem *rectangle;
     QGraphicsSimpleTextItem *text;
};
#endif // MAINWINDOW_H
