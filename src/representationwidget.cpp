#include "representationwidget.h"

#include "representation/representation.hpp"
#include <vector>

#include <QCoreApplication>

#include <GL/freeglut.h>

#include <iostream>

RepresentationWidget::RepresentationWidget(QWidget *parent) : QOpenGLWidget(parent) {

}

void RepresentationWidget::StartRepresentation(std::vector<std::vector<sat>::iterator>& shownSats, station& sta, std::string mapfile, std::string objfile) {
    startRepresentation(shownSats, sta, mapfile, objfile);
}

void RepresentationWidget::initializeGL() {
    initializeOpenGLFunctions();

    int argc = 0;
    char **argv = NULL;
    glutInit(&argc, argv);

    auto glformat = format();
    glformat.setSamples(4);
    setFormat(glformat);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
}

void RepresentationWidget::resizeGL(int w, int h) {
    // Compute aspect ratio of the new window
    if (h == 0) h = 1;                // To prevent divide by 0

    // Set the viewport to cover the new window
    glViewport(0, 0, w, h);
    std::cout << "resize" << std::endl;
}

void RepresentationWidget::paintGL() {

    glBegin(GL_LINES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glEnd();

    renderRepresentation(width(), height());

    std::cout << "paint" << std::endl;
}
