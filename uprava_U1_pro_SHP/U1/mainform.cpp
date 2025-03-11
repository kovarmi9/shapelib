#include "mainform.h"
#include "./ui_mainform.h"
#include "algorithms.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::on_actionPoint_Polygon_triggered()
{
    ui->Canvas->switch_source();
}

void MainForm::on_actionRay_Crossing_triggered()
{
    QPointF q = ui->Canvas->getQ();
    const std::vector<QPolygonF>& polygons = ui->Canvas->getPolygons();

    for (const QPolygonF& pol : polygons)
    {
        short res = Algorithms::analyzeRayCrossing(q, pol);
        if (res == 1)
        {
            setWindowTitle("Inside");
            return;
        }
    }
    setWindowTitle("Outside");
}

void MainForm::on_actionWinding_Number_triggered()
{
    QPointF q = ui->Canvas->getQ();
    const std::vector<QPolygonF>& polygons = ui->Canvas->getPolygons();

    for (const QPolygonF& pol : polygons)
    {
        short res = Algorithms::analyzeWindingNumber(q, pol);
        if (res == 1)
        {
            setWindowTitle("Inside");
            return;
        }
    }
    setWindowTitle("Outside");
}

void MainForm::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Polygon File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty())
    {
        ui->Canvas->loadPolygonFromFile(fileName);
    }
}

void MainForm::on_actionShapefile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Otevřít Shapefile", "", "Shapefile (*.shp);;All Files (*)");
    if (!fileName.isEmpty())
    {
        ui->Canvas->loadPolygonFromShapefile(fileName);
    }
}

void MainForm::on_actionClear_data_triggered()
{
    ui->Canvas->clearPolygons();
}

