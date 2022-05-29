#pragma once

#include <ToolWidgetFactory.h>

namespace SigDigger {
    class SatViewerWidgetFactory;

    class SatGLWidget : public QOpenGLWidget {
        Q_OBJECT

        SUFLOAT updatePeriod = 1000.0f;
        std::string mapfile = "map.json";

        bool tleCorrection  = false;
        bool isSatellite    = false;
        std::string satName = "";
        std::string tleData = "";

    public:
        SatViewerWidget(SatViewerWidgetFactory *, UIMediator *, QWidget *parent = nullptr);
        ~SatViewerWidget() override;

        Suscan::Serializable *allocConfig() override;
        void applyConfig() override;
        bool event(QEvent *) override;

        void setQth(Suscan::Location const &) override;

    public slots:
        // Notifications
        void onSetTLE(Suscan::InspectorMessage const &);
        void onOrbitReport(Suscan::InspectorMessage const &);
    };

    class SatViewerWidget : public ToolWidget {
        Q_OBJECT
        SatViewerWidgetConfig *panelConfig = nullptr;

        // UI members
        SatGLWidget *satGL = nullptr;
        FrequencyCorrectionDialog *fcDialog = nullptr;

    public:
        SatViewerWidget(SatViewerWidgetFactory *, UIMediator *, QWidget *parent = nullptr);
        ~SatViewerWidget() override;
        bool event(QEvent *) override;

    public slots:
        // Notifications
        void onSetTLE(Suscan::InspectorMessage const &);
        void onOrbitReport(Suscan::InspectorMessage const &);
    };

}