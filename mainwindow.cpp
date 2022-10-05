#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <vector>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    sceneZ = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(sceneZ);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calcBttn_clicked()
{
// TODO add all func in class

// get coords
    target_coord[0] = ui->target_get_x->value();
    target_coord[1] = ui->target_get_y->value();
    target_coord[2] = ui->target_get_z->value();
    uav_coord[0] = ui->uav_get_x->value();
    uav_coord[1] = ui->uav_get_y->value();
    uav_coord[2] = ui->uav_get_z->value();
    uav_coord[3] = ui->uav_get_angle->value();
    scale_x = ui->get_map_scale->value();

//tf to uav coordinates
    target_coord_in_uav[0] = target_coord[0] - uav_coord[0];
    target_coord_in_uav[1] = target_coord[1] - uav_coord[1];
//check direction
    if(uav_coord[1] > target_coord[1]){
        camera_xy_change_dir = true;
    } else {
        camera_xy_change_dir = false;
    }
// calc xy coord
    calcVectorSub(v, target_coord_in_uav);
    normalizeVector(v_normalize, v);
    camera_coord[0] = calcAngleVectors(v_normalize, a_normalize, camera_xy_change_dir);

// calc z coord
    calcVectorSub(w, 0, 0, v[0], uav_coord[2] - target_coord[2]);
    normalizeVector(w_normalize, w);
    camera_coord[1] = calcAngleVectors(w_normalize, az_normalize, camera_xy_change_dir);
//merge with uav angle
    camera_coord[0] = (uav_coord[3] - camera_coord[0]) * -1;

    ui->target_x->setText(QString::number(camera_coord[0]));
    ui->target_y->setText(QString::number(camera_coord[1]));

// TODO add uav direction
//show info in frst scene
    scene->clear();
    text = scene->addSimpleText("Target");
    text ->setPos((target_coord_in_uav[0] * scale_x )+ 10, -(target_coord_in_uav[1] * scale_x) - 10);
    text = scene->addSimpleText("N");
    text ->setPos(150 + 10, 10);
    camera_x_line = scene->addLine(0, 150 , 0, -150, QColorConstants::DarkRed);
    camera_x_line = scene->addLine(-150, 0 , 150, 0, QColorConstants::DarkGreen);
    ellipse = scene->addEllipse( (target_coord_in_uav[0] * scale_x)- 5, -(target_coord_in_uav[1]* scale_x) - 5, 10, 10);
    rectangle = scene->addRect(0 - 5, 0 -5, 10,  10);
    camera_x_line = scene->addLine(0 , 0 , target_coord_in_uav[0] * scale_x, -target_coord_in_uav[1] * scale_x);

//show info in snd scene
    sceneZ->clear();
    text = sceneZ->addSimpleText("UAV");
    text ->setPos(5, -((uav_coord[2] - target_coord[2]) / 2)- 20);
    text = sceneZ->addSimpleText("Target");
    text ->setPos((calcVectorLenght(v) * scale_x)+ 10, -20);
    camera_x_line = sceneZ->addLine(0, -((uav_coord[2] - target_coord[2]) / 2)- 20 , 0, 15, QColorConstants::Green);
    camera_x_line = sceneZ->addLine(-15, 0, (calcVectorLenght(v) * scale_x) + 20, 0, QColorConstants::Blue);
    ellipse = sceneZ->addEllipse((calcVectorLenght(v) * scale_x) - 5, 0 - 5, 10, 10);
    rectangle = sceneZ->addRect(0 - 5, -((uav_coord[2] - target_coord[2]) / 2) - 5, 10,  10);
    camera_x_line = sceneZ->addLine(0, -((uav_coord[2] - target_coord[2]) / 2), (calcVectorLenght(v) * scale_x), 0);
}

void MainWindow::calcVectorSub(std::vector<float>& _out_vector, float _x1, float _y1, float _x2, float _y2)
{
    _out_vector[0] = _x2 - _x1;
    _out_vector[1] = _y2 - _y1;
}
void MainWindow::calcVectorSub(std::vector<float>& _out_vector, std::vector<float>& _a_vector, std::vector<float>& _b_vector)
{
    _out_vector[0] = _b_vector[0] - _a_vector[0];
    _out_vector[1] = _b_vector[1] - _a_vector[1];
}
void MainWindow::calcVectorSub(std::vector<float>& _out_vector, std::vector<float>& _b_vector)
{
    _out_vector[0] = _b_vector[0] - 0;
    _out_vector[1] = _b_vector[1] - 0;
}
float MainWindow::calcVectorLenght(std::vector<float>& _v)
{
    return sqrt(pow(_v[0],2) + pow(_v[1],2));
}
float MainWindow::calcVectorLenght(float _a, float _b)
{
    return sqrt(pow(_a,2) + pow(_b,2));
}

void MainWindow::calcZeroVector(std::vector<float>& _out, std::vector<float>& _in)
{
    _out[0] = _in[0] + 1;
    _out[1] = _in[1];
}

void MainWindow::calcZeroVector(std::vector<float>& _out, float _x, float _y)
{
    _out[0] = _x + 1;
    _out[1] = _y;
}

void MainWindow::normalizeVector(std::vector<float>& _to, std::vector<float>& _from)
{
    _to[0] = _from[0] / sqrt(pow(_from[0],2) + pow(_from[1],2));
    _to[1] = _from[1] / sqrt(pow(_from[0],2) + pow(_from[1],2));
}

float MainWindow::calcAngleVectors(std::vector<float>& _v_normalize, std::vector<float>& _a_normalize, bool _dir)
{
    float angle_bw_vectors = std::acos(_a_normalize[0] * _v_normalize[0] +
                                    _a_normalize[1] * _v_normalize[1]);
    if (_dir) return (angle_bw_vectors * 180.0 / 3.1415) * -1 ;
    return angle_bw_vectors * 180.0 / 3.1415 ;
}
