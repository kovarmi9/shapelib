#include "draw.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "../shapelib/shapefil.h"

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{
    q.setX(0);
    q.setY(0);
    add_point = false;
    isShapefileLoaded = false;
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    double x = e->pos().x();
    double y = e->pos().y();

    if (add_point)
    {
        q.setX(x);
        q.setY(y);
    }
    else
    {
        QPointF p(x, y);
        currentPolygon.push_back(p);
    }

    repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);

    if (!polygons.empty())
    {
        QRectF totalBoundingRect;
        for (const QPolygonF& polygon : polygons)
        {
            totalBoundingRect = totalBoundingRect.united(polygon.boundingRect());
        }

        qreal scaleX = width() / totalBoundingRect.width();
        qreal scaleY = height() / totalBoundingRect.height();
        qreal scale = qMin(scaleX, scaleY);
        qreal offsetX = (width() - totalBoundingRect.width() * scale) / 2;
        qreal offsetY = (height() - totalBoundingRect.height() * scale) / 2;

        painter.translate(offsetX, offsetY);
        painter.scale(scale, scale);
        painter.translate(-totalBoundingRect.topLeft());

        for (const QPolygonF& polygon : polygons)
        {
            painter.drawPolygon(polygon);
        }
    }

    painter.setPen(Qt::GlobalColor::black);
    painter.setBrush(Qt::GlobalColor::blue);
    int r = 5;
    painter.drawEllipse(q.x() - r, q.y() - r, 2 * r, 2 * r);

    painter.end();
}

void Draw::switch_source()
{
    add_point = !add_point;
}

void Draw::loadPolygonFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open file for reading");
        return;
    }

    QTextStream in(&file);
    currentPolygon.clear();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList coordinates = line.split(",");
        if (coordinates.size() == 2)
        {
            bool ok1, ok2;
            double x = coordinates[0].toDouble(&ok1);
            double y = coordinates[1].toDouble(&ok2);
            if (ok1 && ok2)
            {
                currentPolygon.append(QPointF(x, y));
            }
        }
    }

    file.close();
    polygons.push_back(currentPolygon); // Přidání polygonu do vektoru
    repaint();
}

void Draw::loadPolygonFromShapefile(const QString &fileName)
{
    SHPHandle hSHP = SHPOpen(fileName.toStdString().c_str(), "rb");
    if (hSHP == nullptr)
    {
        QMessageBox::warning(this, "Error", "Cannot open shapefile.");
        return;
    }

    int nEntities, nShapeType;
    SHPGetInfo(hSHP, &nEntities, &nShapeType, nullptr, nullptr);

    polygons.clear();

    for (int i = 0; i < nEntities; ++i)
    {
        SHPObject *psShape = SHPReadObject(hSHP, i);
        if (psShape == nullptr)
        {
            continue;
        }

        QPolygonF polygon;
        for (int j = 0; j < psShape->nVertices; ++j)
        {
            polygon.append(QPointF(psShape->padfX[j], psShape->padfY[j]));
        }

        polygons.push_back(polygon);
        SHPDestroyObject(psShape);
    }

    SHPClose(hSHP);

    isShapefileLoaded = true;
    repaint();
}

void Draw::clearPolygons()
{
    currentPolygon.clear();
    polygons.clear();
    isShapefileLoaded = false;
    repaint();
}
