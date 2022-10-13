#ifndef REPRESENTATIONWIDGET_H
#define REPRESENTATIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <representation/representation.hpp>

class RepresentationWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    RepresentationWidget(QWidget *parent);
    void StartRepresentation(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile, std::string objfile);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // REPRESENTATIONWIDGET_H
